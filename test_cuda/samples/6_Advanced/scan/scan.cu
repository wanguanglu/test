/*
 * Copyright 1993-2015 NVIDIA Corporation.  All rights reserved.
 *
 * Please refer to the NVIDIA end user license agreement (EULA) associated
 * with this source code for terms and conditions that govern your use of
 * this software. Any use, reproduction, disclosure, or distribution of
 * this software and related documentation outside the terms of the EULA
 * is strictly prohibited.
 *
 */

#include "scan_common.h"
#include <assert.h>
#include <helper_cuda.h>

// All three kernels run 512 threads per workgroup
// Must be a power of two
#define THREADBLOCK_SIZE 256

////////////////////////////////////////////////////////////////////////////////
// Basic scan codelets
////////////////////////////////////////////////////////////////////////////////
// Naive inclusive scan: O(N * log2(N)) operations
// Allocate 2 * 'size' local memory, initialize the first half
// with 'size' zeros avoiding if(pos >= offset) condition evaluation
// and saving instructions
inline __device__ uint scan1Inclusive(uint idata, volatile uint* s_Data,
                                      uint size) {
  uint pos = 2 * threadIdx.x - (threadIdx.x & (size - 1));
  s_Data[pos] = 0;
  pos += size;
  s_Data[pos] = idata;

  for (uint offset = 1; offset < size; offset <<= 1) {
    __syncthreads();
    uint t = s_Data[pos] + s_Data[pos - offset];
    __syncthreads();
    s_Data[pos] = t;
  }

  return s_Data[pos];
}

inline __device__ uint scan1Exclusive(uint idata, volatile uint* s_Data,
                                      uint size) {
  return scan1Inclusive(idata, s_Data, size) - idata;
}

inline __device__ uint4 scan4Inclusive(uint4 idata4, volatile uint* s_Data,
                                       uint size) {
  // Level-0 inclusive scan
  idata4.y += idata4.x;
  idata4.z += idata4.y;
  idata4.w += idata4.z;

  // Level-1 exclusive scan
  uint oval = scan1Exclusive(idata4.w, s_Data, size / 4);

  idata4.x += oval;
  idata4.y += oval;
  idata4.z += oval;
  idata4.w += oval;

  return idata4;
}

// Exclusive vector scan: the array to be scanned is stored
// in local thread memory scope as uint4
inline __device__ uint4 scan4Exclusive(uint4 idata4, volatile uint* s_Data,
                                       uint size) {
  uint4 odata4 = scan4Inclusive(idata4, s_Data, size);
  odata4.x -= idata4.x;
  odata4.y -= idata4.y;
  odata4.z -= idata4.z;
  odata4.w -= idata4.w;
  return odata4;
}

////////////////////////////////////////////////////////////////////////////////
// Scan kernels
////////////////////////////////////////////////////////////////////////////////
__global__ void scanExclusiveShared(uint4* d_Dst, uint4* d_Src, uint size) {
  __shared__ uint s_Data[2 * THREADBLOCK_SIZE];

  uint pos = blockIdx.x * blockDim.x + threadIdx.x;

  // Load data
  uint4 idata4 = d_Src[pos];

  // Calculate exclusive scan
  uint4 odata4 = scan4Exclusive(idata4, s_Data, size);

  // Write back
  d_Dst[pos] = odata4;
}

// Exclusive scan of top elements of bottom-level scans (4 * THREADBLOCK_SIZE)
__global__ void scanExclusiveShared2(uint* d_Buf, uint* d_Dst, uint* d_Src,
                                     uint N, uint arrayLength) {
  __shared__ uint s_Data[2 * THREADBLOCK_SIZE];

  // Skip loads and stores for inactive threads of last threadblock (pos >= N)
  uint pos = blockIdx.x * blockDim.x + threadIdx.x;

  // Load top elements
  // Convert results of bottom-level scan back to inclusive
  uint idata = 0;

  if (pos < N)
    idata = d_Dst[(4 * THREADBLOCK_SIZE) - 1 + (4 * THREADBLOCK_SIZE) * pos] +
            d_Src[(4 * THREADBLOCK_SIZE) - 1 + (4 * THREADBLOCK_SIZE) * pos];

  // Compute
  uint odata = scan1Exclusive(idata, s_Data, arrayLength);

  // Avoid out-of-bound access
  if (pos < N) {
    d_Buf[pos] = odata;
  }
}

// Final step of large-array scan: combine basic inclusive scan with exclusive
// scan of top elements of input arrays
__global__ void uniformUpdate(uint4* d_Data, uint* d_Buffer) {
  __shared__ uint buf;
  uint pos = blockIdx.x * blockDim.x + threadIdx.x;

  if (threadIdx.x == 0) {
    buf = d_Buffer[blockIdx.x];
  }

  __syncthreads();

  uint4 data4 = d_Data[pos];
  data4.x += buf;
  data4.y += buf;
  data4.z += buf;
  data4.w += buf;
  d_Data[pos] = data4;
}

////////////////////////////////////////////////////////////////////////////////
// Interface function
////////////////////////////////////////////////////////////////////////////////
// Derived as 32768 (max power-of-two gridDim.x) * 4 * THREADBLOCK_SIZE
// Due to scanExclusiveShared<<<>>>() 1D block addressing
extern "C" const uint MAX_BATCH_ELEMENTS = 64 * 1048576;
extern "C" const uint MIN_SHORT_ARRAY_SIZE = 4;
extern "C" const uint MAX_SHORT_ARRAY_SIZE = 4 * THREADBLOCK_SIZE;
extern "C" const uint MIN_LARGE_ARRAY_SIZE = 8 * THREADBLOCK_SIZE;
extern "C" const uint MAX_LARGE_ARRAY_SIZE =
    4 * THREADBLOCK_SIZE * THREADBLOCK_SIZE;

// Internal exclusive scan buffer
static uint* d_Buf;

extern "C" void initScan(void) {
  checkCudaErrors(
      cudaMalloc((void**)&d_Buf,
                 (MAX_BATCH_ELEMENTS / (4 * THREADBLOCK_SIZE)) * sizeof(uint)));
}

extern "C" void closeScan(void) { checkCudaErrors(cudaFree(d_Buf)); }

static uint factorRadix2(uint& log2L, uint L) {
  if (!L) {
    log2L = 0;
    return 0;
  } else {
    for (log2L = 0; (L & 1) == 0; L >>= 1, log2L++)
      ;

    return L;
  }
}

static uint iDivUp(uint dividend, uint divisor) {
  return ((dividend % divisor) == 0) ? (dividend / divisor)
                                     : (dividend / divisor + 1);
}

extern "C" size_t scanExclusiveShort(uint* d_Dst, uint* d_Src, uint batchSize,
                                     uint arrayLength) {
  // Check power-of-two factorization
  uint log2L;
  uint factorizationRemainder = factorRadix2(log2L, arrayLength);
  assert(factorizationRemainder == 1);

  // Check supported size range
  assert((arrayLength >= MIN_SHORT_ARRAY_SIZE) &&
         (arrayLength <= MAX_SHORT_ARRAY_SIZE));

  // Check total batch size limit
  assert((batchSize * arrayLength) <= MAX_BATCH_ELEMENTS);

  // Check all threadblocks to be fully packed with data
  assert((batchSize * arrayLength) % (4 * THREADBLOCK_SIZE) == 0);

  scanExclusiveShared<<<(batchSize * arrayLength) / (4 * THREADBLOCK_SIZE),
                        THREADBLOCK_SIZE>>>((uint4*)d_Dst, (uint4*)d_Src,
                                            arrayLength);
  getLastCudaError("scanExclusiveShared() execution FAILED\n");

  return THREADBLOCK_SIZE;
}

extern "C" size_t scanExclusiveLarge(uint* d_Dst, uint* d_Src, uint batchSize,
                                     uint arrayLength) {
  // Check power-of-two factorization
  uint log2L;
  uint factorizationRemainder = factorRadix2(log2L, arrayLength);
  assert(factorizationRemainder == 1);

  // Check supported size range
  assert((arrayLength >= MIN_LARGE_ARRAY_SIZE) &&
         (arrayLength <= MAX_LARGE_ARRAY_SIZE));

  // Check total batch size limit
  assert((batchSize * arrayLength) <= MAX_BATCH_ELEMENTS);

  scanExclusiveShared<<<(batchSize * arrayLength) / (4 * THREADBLOCK_SIZE),
                        THREADBLOCK_SIZE>>>((uint4*)d_Dst, (uint4*)d_Src,
                                            4 * THREADBLOCK_SIZE);
  getLastCudaError("scanExclusiveShared() execution FAILED\n");

  // Not all threadblocks need to be packed with input data:
  // inactive threads of highest threadblock just don't do global reads and
  // writes
  const uint blockCount2 = iDivUp(
      (batchSize * arrayLength) / (4 * THREADBLOCK_SIZE), THREADBLOCK_SIZE);
  scanExclusiveShared2<<<blockCount2, THREADBLOCK_SIZE>>>(
      (uint*)d_Buf, (uint*)d_Dst, (uint*)d_Src,
      (batchSize * arrayLength) / (4 * THREADBLOCK_SIZE),
      arrayLength / (4 * THREADBLOCK_SIZE));
  getLastCudaError("scanExclusiveShared2() execution FAILED\n");

  uniformUpdate<<<(batchSize * arrayLength) / (4 * THREADBLOCK_SIZE),
                  THREADBLOCK_SIZE>>>((uint4*)d_Dst, (uint*)d_Buf);
  getLastCudaError("uniformUpdate() execution FAILED\n");

  return THREADBLOCK_SIZE;
}
