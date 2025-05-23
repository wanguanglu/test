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

/*
 * Demonstration of inline PTX (assembly language) usage in CUDA kernels
 */

// System includes
#include <assert.h>
#include <stdio.h>

// CUDA runtime
#include <cuda_runtime.h>

// helper functions and utilities to work with CUDA
#include <helper_cuda.h>
#include <helper_functions.h>

__global__ void sequence_gpu(int* d_ptr, int length) {
  int elemID = blockIdx.x * blockDim.x + threadIdx.x;

  if (elemID < length) {
    unsigned int laneid;
    // This command gets the lane ID within the current warp
    asm("mov.u32 %0, %%laneid;" : "=r"(laneid));
    d_ptr[elemID] = laneid;
  }
}

void sequence_cpu(int* h_ptr, int length) {
  for (int elemID = 0; elemID < length; elemID++) {
    h_ptr[elemID] = elemID % 32;
  }
}

int main(int argc, char** argv) {
  printf("CUDA inline PTX assembler sample\n");

  const int N = 1000;

  int dev = findCudaDevice(argc, (const char**)argv);

  if (dev == -1) {
    return EXIT_FAILURE;
  }

  int* d_ptr;
  checkCudaErrors(cudaMalloc(&d_ptr, N * sizeof(int)));

  int* h_ptr;
  checkCudaErrors(cudaMallocHost(&h_ptr, N * sizeof(int)));

  dim3 cudaBlockSize(256, 1, 1);
  dim3 cudaGridSize((N + cudaBlockSize.x - 1) / cudaBlockSize.x, 1, 1);
  sequence_gpu<<<cudaGridSize, cudaBlockSize>>>(d_ptr, N);
  checkCudaErrors(cudaGetLastError());
  checkCudaErrors(cudaDeviceSynchronize());

  sequence_cpu(h_ptr, N);

  int* h_d_ptr;
  checkCudaErrors(cudaMallocHost(&h_d_ptr, N * sizeof(int)));
  checkCudaErrors(
      cudaMemcpy(h_d_ptr, d_ptr, N * sizeof(int), cudaMemcpyDeviceToHost));

  bool bValid = true;

  for (int i = 0; i < N && bValid; i++) {
    if (h_ptr[i] != h_d_ptr[i]) {
      bValid = false;
    }
  }

  printf("Test %s.\n", bValid ? "Successful" : "Failed");

  checkCudaErrors(cudaFree(d_ptr));
  checkCudaErrors(cudaFreeHost(h_ptr));
  checkCudaErrors(cudaFreeHost(h_d_ptr));

  // cudaDeviceReset causes the driver to clean up all state. While
  // not mandatory in normal operation, it is good practice.  It is also
  // needed to ensure correct operation when the application is being
  // profiled. Calling cudaDeviceReset causes all profile data to be
  // flushed before the application exits
  checkCudaErrors(cudaDeviceReset());

  return bValid ? EXIT_SUCCESS : EXIT_FAILURE;
}
