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

// This example shows how to use the clock function to measure the performance
// of a kernel accurately.
//

// Blocks are executed in parallel and out of order. Since there's no
// synchronization mechanism between blocks, we measure the clock once for each
// block. The clock samples are written to device memory.

// System includes
#include <assert.h>
#include <stdint.h>
#include <stdio.h>

#include <cudaProfiler.h>
#include <cuda_runtime.h>
#include <nvrtc_helper.h>

// helper functions and utilities to work with CUDA
#include <helper_functions.h>

#define NUM_BLOCKS 64

#define NUM_THREADS 256

// It's interesting to change the number of blocks and the number of threads to
// understand how to keep the hardware busy.
//

// Here are some numbers I get on my G80:
//    blocks - clocks
//    1 - 3096
//    8 - 3232
//    16 - 3364
//    32 - 4615
//    64 - 9981

//
// With less than 16 blocks some of the multiprocessors of the device are idle.
// With more than 16 you are using all the multiprocessors, but there's only one
// block per multiprocessor and that doesn't allow you to hide the latency of
// the memory. With more than 32 the speed scales linearly.

// Start the main CUDA Sample here

int main(int argc, char** argv) {
  printf("CUDA Clock sample\n");

  typedef long clock_t;

  clock_t timer[NUM_BLOCKS * 2];

  float input[NUM_THREADS * 2];

  for (int i = 0; i < NUM_THREADS * 2; i++) {
    input[i] = (float)i;
  }

  char *ptx, *kernel_file;
  size_t ptxSize;

  kernel_file = sdkFindFilePath("clock_kernel.cu", argv[0]);
  compileFileToPTX(kernel_file, 0, NULL, &ptx, &ptxSize);

  CUmodule module = loadPTX(ptx, argc, argv);
  CUfunction kernel_addr;

  checkCudaErrors(cuModuleGetFunction(&kernel_addr, module, "timedReduction"));

  dim3 cudaBlockSize(NUM_THREADS, 1, 1);
  dim3 cudaGridSize(NUM_BLOCKS, 1, 1);

  CUdeviceptr dinput, doutput, dtimer;
  checkCudaErrors(cuMemAlloc(&dinput, sizeof(float) * NUM_THREADS * 2));
  checkCudaErrors(cuMemAlloc(&doutput, sizeof(float) * NUM_BLOCKS));
  checkCudaErrors(cuMemAlloc(&dtimer, sizeof(clock_t) * NUM_BLOCKS * 2));
  checkCudaErrors(cuMemcpyHtoD(dinput, input, sizeof(float) * NUM_THREADS * 2));

  void* arr[] = {(void*)&dinput, (void*)&doutput, (void*)&dtimer};

  checkCudaErrors(cuLaunchKernel(
      kernel_addr, cudaGridSize.x, cudaGridSize.y,
      cudaGridSize.z,                                    /* grid dim */
      cudaBlockSize.x, cudaBlockSize.y, cudaBlockSize.z, /* block dim */
      sizeof(float) * 2 * NUM_THREADS, 0, /* shared mem, stream */
      &arr[0],                            /* arguments */
      0));

  checkCudaErrors(cuCtxSynchronize());
  checkCudaErrors(
      cuMemcpyDtoH(timer, dtimer, sizeof(clock_t) * NUM_BLOCKS * 2));
  checkCudaErrors(cuMemFree(dinput));
  checkCudaErrors(cuMemFree(doutput));
  checkCudaErrors(cuMemFree(dtimer));

  // Compute the difference between the last block end and the first block
  // start.
  clock_t minStart = timer[0];
  clock_t maxEnd = timer[NUM_BLOCKS];

  for (int i = 1; i < NUM_BLOCKS; i++) {
    minStart = timer[i] < minStart ? timer[i] : minStart;
    maxEnd = timer[NUM_BLOCKS + i] > maxEnd ? timer[NUM_BLOCKS + i] : maxEnd;
  }

  printf("Total clocks = %Lf\n", (long double)(maxEnd - minStart));

  cuProfilerStop();

  return EXIT_SUCCESS;
}
