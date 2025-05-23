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
//
// This sample demonstrates dynamic global memory allocation through device C++
// new and delete operators and virtual function declarations available with
// CUDA 4.0.

#include <stdio.h>

#include <helper_cuda.h>

#include <stdlib.h>

#include <algorithm>
#include <vector>

const char* sSDKsample = "newdelete";

#include "container.hpp"

/////////////////////////////////////////////////////////////////////////////
//
// Kernels to allocate and instantiate Container objects on the device heap
//
////////////////////////////////////////////////////////////////////////////

__global__ void vectorCreate(Container<int>** g_container, int max_size) {
  // The Vector object and the data storage are allocated in device heap memory.
  // This makes it persistent for the lifetime of the CUDA context.
  // The grid has only one thread as only a single object instance is needed.

  *g_container = new Vector<int>(max_size);
}

/////////////////////////////////////////////////////////////////////////////
//
// Kernels to fill and consume shared Container objects.
//
////////////////////////////////////////////////////////////////////////////

__global__ void containerFill(Container<int>** g_container) {
  // All threads of the grid cooperatively populate the shared Container object
  // with data.
  if (threadIdx.x == 0) {
    (*g_container)->push(blockIdx.x);
  }
}

__global__ void containerConsume(Container<int>** g_container, int* d_result) {
  // All threads of the grid cooperatively consume the data from the shared
  // Container object.
  int idx = blockIdx.x * blockDim.x + threadIdx.x;

  int v;

  if ((*g_container)->pop(v)) {
    d_result[idx] = v;
  } else {
    d_result[idx] = -1;
  }
}

/////////////////////////////////////////////////////////////////////////////
//
// Kernel to delete shared Container objects.
//
////////////////////////////////////////////////////////////////////////////

__global__ void containerDelete(Container<int>** g_container) {
  delete *g_container;
}

///////////////////////////////////////////////////////////////////////////////////////////
//
// Kernels to using of placement new to put shared Vector objects and data in
// shared memory
//
///////////////////////////////////////////////////////////////////////////////////////////

__global__ void placementNew(int* d_result) {
  __shared__ unsigned char __align__(8) s_buffer[sizeof(Vector<int>)];
  __shared__ int __align__(8) s_data[1024];
  __shared__ Vector<int>* s_vector;

  // The first thread of the block initializes the shared Vector object.
  // The placement new operator enables the Vector object and the data array top
  // be placed in shared memory.
  if (threadIdx.x == 0) {
    s_vector = new (s_buffer) Vector<int>(1024, s_data);
  }

  __syncthreads();

  if ((threadIdx.x & 1) == 0) {
    s_vector->push(threadIdx.x >> 1);
  }

  // Need to sync as the vector implementation does not support concurrent
  // push/pop operations.
  __syncthreads();

  int v;

  if (s_vector->pop(v)) {
    d_result[threadIdx.x] = v;
  } else {
    d_result[threadIdx.x] = -1;
  }

  // Note: deleting objects placed in shared memory is not necessary (lifetime
  // of shared memory is that of the block)
}

struct ComplexType_t {
  int a;
  int b;
  float c;
  float d;
};

__global__ void complexVector(int* d_result) {
  __shared__ unsigned char __align__(8) s_buffer[sizeof(Vector<ComplexType_t>)];
  __shared__ ComplexType_t __align__(8) s_data[1024];
  __shared__ Vector<ComplexType_t>* s_vector;

  // The first thread of the block initializes the shared Vector object.
  // The placement new operator enables the Vector object and the data array top
  // be placed in shared memory.
  if (threadIdx.x == 0) {
    s_vector = new (s_buffer) Vector<ComplexType_t>(1024, s_data);
  }

  __syncthreads();

  if ((threadIdx.x & 1) == 0) {
    ComplexType_t data;
    data.a = threadIdx.x >> 1;
    data.b = blockIdx.x;
    data.c = threadIdx.x / (float)(blockDim.x);
    data.d = blockIdx.x / (float)(gridDim.x);

    s_vector->push(data);
  }

  __syncthreads();

  ComplexType_t v;

  if (s_vector->pop(v)) {
    d_result[threadIdx.x] = v.a;
  } else {
    d_result[threadIdx.x] = -1;
  }

  // Note: deleting objects placed in shared memory is not necessary (lifetime
  // of shared memory is that of the block)
}

///////////////////////////////////////////////////////////////////////////////////////////
//
// Host code
//
///////////////////////////////////////////////////////////////////////////////////////////

bool checkResult(int* d_result, int N) {
  std::vector<int> h_result;
  h_result.resize(N);

  checkCudaErrors(cudaMemcpy(&h_result[0], d_result, N * sizeof(int),
                             cudaMemcpyDeviceToHost));
  std::sort(h_result.begin(), h_result.end());

  bool success = true;
  bool test = false;

  int value = 0;

  for (int i = 0; i < N; ++i) {
    if (h_result[i] != -1) {
      test = true;
    }

    if (test && (value++) != h_result[i]) {
      success = false;
    }
  }

  return success;
}

bool testContainer(Container<int>** d_container, int blocks, int threads) {
  int* d_result;
  cudaMalloc(&d_result, blocks * threads * sizeof(int));

  containerFill<<<blocks, threads>>>(d_container);
  containerConsume<<<blocks, threads>>>(d_container, d_result);
  containerDelete<<<1, 1>>>(d_container);
  checkCudaErrors(cudaDeviceSynchronize());

  bool success = checkResult(d_result, blocks * threads);

  cudaFree(d_result);

  return success;
}

bool testPlacementNew(int threads) {
  int* d_result;
  cudaMalloc(&d_result, threads * sizeof(int));

  placementNew<<<1, threads>>>(d_result);
  checkCudaErrors(cudaDeviceSynchronize());

  bool success = checkResult(d_result, threads);

  cudaFree(d_result);

  return success;
}

bool testComplexType(int threads) {
  int* d_result;
  cudaMalloc(&d_result, threads * sizeof(int));

  complexVector<<<1, threads>>>(d_result);
  checkCudaErrors(cudaDeviceSynchronize());

  bool success = checkResult(d_result, threads);

  cudaFree(d_result);

  return success;
}

///////////////////////////////////////////////////////////////////////////////////////////
//
// MAIN
//
///////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {
  int cuda_device = 0;

  printf("%s Starting...\n\n", sSDKsample);

  // use command-line specified CUDA device, otherwise use device with highest
  // Gflops/s
  cuda_device = findCudaDevice(argc, (const char**)argv);

  cudaDeviceProp deviceProp;
  checkCudaErrors(cudaGetDevice(&cuda_device));
  checkCudaErrors(cudaGetDeviceProperties(&deviceProp, cuda_device));

  if (deviceProp.major < 2) {
    printf(
        "> This GPU with Compute Capability %d.%d does not meet minimum "
        "requirements.\n",
        deviceProp.major, deviceProp.minor);
    printf("> A GPU with Compute Capability >= 2.0 is required to run %s.\n",
           sSDKsample);
    printf("> Test will not run.  Exiting.\n");
    exit(EXIT_SUCCESS);
  }

  // set the heap size for device size new/delete to 128 MB
#if CUDART_VERSION >= 4000
  cudaDeviceSetLimit(cudaLimitMallocHeapSize, 128 * (1 << 20));
#else
  cudaThreadSetLimit(cudaLimitMallocHeapSize, 128 * (1 << 20));
#endif

  Container<int>** d_container;
  cudaMalloc(&d_container, sizeof(Container<int>**));

  bool bTest = false;
  int test_passed = 0;

  printf(" > Container = Vector test ");
  vectorCreate<<<1, 1>>>(d_container, 128 * 128);
  bTest = testContainer(d_container, 128, 128);
  printf(bTest ? "OK\n\n" : "NOT OK\n\n");
  test_passed += (bTest ? 1 : 0);

  cudaFree(d_container);

  printf(" > Container = Vector, using placement new on SMEM buffer test ");
  bTest = testPlacementNew(1024);
  printf(bTest ? "OK\n\n" : "NOT OK\n\n");
  test_passed += (bTest ? 1 : 0);

  printf(" > Container = Vector, with user defined datatype test ");
  bTest = testComplexType(1024);
  printf(bTest ? "OK\n\n" : "NOT OK\n\n");
  test_passed += (bTest ? 1 : 0);

  printf("Test Summary: %d/3 succesfully run\n", test_passed);

  // cudaDeviceReset causes the driver to clean up all state. While
  // not mandatory in normal operation, it is good practice.  It is also
  // needed to ensure correct operation when the application is being
  // profiled. Calling cudaDeviceReset causes all profile data to be
  // flushed before the application exits
  cudaDeviceReset();
  exit(test_passed == 3 ? EXIT_SUCCESS : EXIT_FAILURE);
};
