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
// This sample demonstrates how HyperQ allows supporting devices to avoid false
// dependencies between kernels in different streams.
//
// - Devices without HyperQ will run a maximum of two kernels at a time (one
//   kernel_A and one kernel_B).
// - Devices with HyperQ will run up to 32 kernels simultaneously.

#include <helper_cuda.h>
#include <helper_functions.h>
#include <stdio.h>

const char* sSDKsample = "hyperQ";

// This subroutine does no real work but runs for at least the specified number
// of clock ticks.
__device__ void clock_block(clock_t* d_o, clock_t clock_count) {
  unsigned int start_clock = (unsigned int)clock();

  clock_t clock_offset = 0;

  while (clock_offset < clock_count) {
    unsigned int end_clock = (unsigned int)clock();

    // The code below should work like
    // this (thanks to modular arithmetics):
    //
    // clock_offset = (clock_t) (end_clock > start_clock ?
    //                           end_clock - start_clock :
    //                           end_clock + (0xffffffffu - start_clock));
    //
    // Indeed, let m = 2^32 then
    // end - start = end + m - start (mod m).

    clock_offset = (clock_t)(end_clock - start_clock);
  }

  d_o[0] = clock_offset;
}

// We create two identical kernels calling clock_block(), we create two so that
// we can identify dependencies in the profile timeline ("kernel_B" is always
// dependent on "kernel_A" in the same stream).
__global__ void kernel_A(clock_t* d_o, clock_t clock_count) {
  clock_block(d_o, clock_count);
}
__global__ void kernel_B(clock_t* d_o, clock_t clock_count) {
  clock_block(d_o, clock_count);
}

// Single-warp reduction kernel (note: this is not optimized for simplicity)
__global__ void sum(clock_t* d_clocks, int N) {
  __shared__ clock_t s_clocks[32];

  clock_t my_sum = 0;

  for (int i = threadIdx.x; i < N; i += blockDim.x) {
    my_sum += d_clocks[i];
  }

  s_clocks[threadIdx.x] = my_sum;
  __syncthreads();

  for (int i = warpSize / 2; i > 0; i /= 2) {
    if (threadIdx.x < i) {
      s_clocks[threadIdx.x] += s_clocks[threadIdx.x + i];
    }

    __syncthreads();
  }

  if (threadIdx.x == 0) {
    d_clocks[0] = s_clocks[0];
  }
}

int main(int argc, char** argv) {
  int nstreams = 32;       // One stream for each pair of kernels
  float kernel_time = 10;  // Time each kernel should run in ms
  float elapsed_time;
  int cuda_device = 0;

  printf("starting %s...\n", sSDKsample);

  // Get number of streams (if overridden on the command line)
  if (checkCmdLineFlag(argc, (const char**)argv, "nstreams")) {
    nstreams = getCmdLineArgumentInt(argc, (const char**)argv, "nstreams");
  }

  // Use command-line specified CUDA device, otherwise use device with
  // highest Gflops/s
  cuda_device = findCudaDevice(argc, (const char**)argv);

  // Get device properties
  cudaDeviceProp deviceProp;
  checkCudaErrors(cudaGetDevice(&cuda_device));
  checkCudaErrors(cudaGetDeviceProperties(&deviceProp, cuda_device));

  // HyperQ is available in devices of Compute Capability 3.5 and higher
  if (deviceProp.major < 3 || (deviceProp.major == 3 && deviceProp.minor < 5)) {
    if (deviceProp.concurrentKernels == 0) {
      printf(
          "> GPU does not support concurrent kernel execution (SM 3.5 or "
          "higher required)\n");
      printf("  CUDA kernel runs will be serialized\n");
    } else {
      printf("> GPU does not support HyperQ\n");
      printf("  CUDA kernel runs will have limited concurrency\n");
    }
  }

  printf("> Detected Compute SM %d.%d hardware with %d multi-processors\n",
         deviceProp.major, deviceProp.minor, deviceProp.multiProcessorCount);

  // Allocate host memory for the output (reduced to a single value)
  clock_t* a = 0;
  checkCudaErrors(cudaMallocHost((void**)&a, sizeof(clock_t)));

  // Allocate device memory for the output (one value for each kernel)
  clock_t* d_a = 0;
  checkCudaErrors(cudaMalloc((void**)&d_a, 2 * nstreams * sizeof(clock_t)));

  // Allocate and initialize an array of stream handles
  cudaStream_t* streams =
      (cudaStream_t*)malloc(nstreams * sizeof(cudaStream_t));

  for (int i = 0; i < nstreams; i++) {
    checkCudaErrors(cudaStreamCreate(&(streams[i])));
  }

  // Create CUDA event handles
  cudaEvent_t start_event, stop_event;
  checkCudaErrors(cudaEventCreate(&start_event));
  checkCudaErrors(cudaEventCreate(&stop_event));

  // Target time per kernel is kernel_time ms, clockRate is in KHz
  // Target number of clocks = target time * clock frequency
  clock_t time_clocks = (clock_t)(kernel_time * deviceProp.clockRate);
  clock_t total_clocks = 0;

  // Start the clock
  checkCudaErrors(cudaEventRecord(start_event, 0));

  // Queue pairs of {kernel_A, kernel_B} in separate streams
  for (int i = 0; i < nstreams; ++i) {
    kernel_A<<<1, 1, 0, streams[i]>>>(&d_a[2 * i], time_clocks);
    total_clocks += time_clocks;
    kernel_B<<<1, 1, 0, streams[i]>>>(&d_a[2 * i + 1], time_clocks);
    total_clocks += time_clocks;
  }

  // Stop the clock in stream 0 (i.e. all previous kernels will be complete)
  checkCudaErrors(cudaEventRecord(stop_event, 0));

  // At this point the CPU has dispatched all work for the GPU and can
  // continue processing other tasks in parallel. In this sample we just want
  // to wait until all work is done so we use a blocking cudaMemcpy below.

  // Run the sum kernel and copy the result back to host
  sum<<<1, 32>>>(d_a, 2 * nstreams);
  checkCudaErrors(cudaMemcpy(a, d_a, sizeof(clock_t), cudaMemcpyDeviceToHost));

  // stop_event will have been recorded but including the synchronize here to
  // prevent copy/paste errors!
  checkCudaErrors(cudaEventSynchronize(stop_event));
  checkCudaErrors(cudaEventElapsedTime(&elapsed_time, start_event, stop_event));

  printf(
      "Expected time for serial execution of %d sets of kernels is between "
      "approx. %.3fs and %.3fs\n",
      nstreams, (nstreams + 1) * kernel_time / 1000.0f,
      2 * nstreams * kernel_time / 1000.0f);
  printf(
      "Expected time for fully concurrent execution of %d sets of kernels "
      "is approx. %.3fs\n",
      nstreams, 2 * kernel_time / 1000.0f);
  printf("Measured time for sample = %.3fs\n", elapsed_time / 1000.0f);

  bool bTestResult = (a[0] >= total_clocks);

  // Release resources
  for (int i = 0; i < nstreams; i++) {
    cudaStreamDestroy(streams[i]);
  }

  free(streams);
  cudaEventDestroy(start_event);
  cudaEventDestroy(stop_event);
  cudaFreeHost(a);
  cudaFree(d_a);

  // cudaDeviceReset causes the driver to clean up all state. While
  // not mandatory in normal operation, it is good practice.  It is also
  // needed to ensure correct operation when the application is being
  // profiled. Calling cudaDeviceReset causes all profile data to be
  // flushed before the application exits
  cudaDeviceReset();
  exit(bTestResult ? EXIT_SUCCESS : EXIT_FAILURE);
}
