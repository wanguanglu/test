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

/* Example of program using the interval_gpu<T> template class and operators:
 * Search for roots of a function using an interval Newton method.
 *
 * Use the command-line argument "--n=<N>" to select which GPU implementation to
 * use, otherwise the naive implementation will be used by default. 0: the naive
 * implementation 1: the optimized implementation 2: the recursive
 * implementation
 *
 */

const static char* sSDKsample = "Interval Computing";

#include "cpu_interval.h"
#include "cuda_interval.h"
#include "helper_cuda.h"
#include "interval.h"
#include <iostream>
#include <stdio.h>

int main(int argc, char* argv[]) {
  int implementation_choice = 0;

  printf("[%s]  starting ...\n\n", sSDKsample);

  if (checkCmdLineFlag(argc, (const char**)argv, "n")) {
    implementation_choice =
        getCmdLineArgumentInt(argc, (const char**)argv, "n");
  }

  // Pick the best GPU available, or if the developer selects one at the command
  // line
  int devID = findCudaDevice(argc, (const char**)argv);
  cudaDeviceProp deviceProp;
  cudaGetDeviceProperties(&deviceProp, devID);
  printf("> GPU Device has Compute Capabilities SM %d.%d\n\n", deviceProp.major,
         deviceProp.minor);

  int version = (deviceProp.major * 0x10 + deviceProp.minor);

  if (version < 0x13) {
    printf(
        "%s: requires minimum of Compute Capability 1.3 or higher, waiving "
        "test...\n",
        sSDKsample);

    // cudaDeviceReset causes the driver to clean up all state. While
    // not mandatory in normal operation, it is good practice.  It is also
    // needed to ensure correct operation when the application is being
    // profiled. Calling cudaDeviceReset causes all profile data to be
    // flushed before the application exits
    cudaDeviceReset();
    exit(EXIT_SUCCESS);
  }

  switch (implementation_choice) {
    case 0:
      printf("GPU naive implementation\n");
      break;

    case 1:
      printf("GPU optimized implementation\n");
      break;

    case 2:
      if (deviceProp.major >= 2) {
        printf("GPU recursive implementation (requires Compute SM 2.0+)\n");
      } else {
        printf(
            "GPU naive implementation is used instead of the recursive "
            "implementation, which requires a GPU with CUDA capability 2.0+\n");
        implementation_choice = 0;
      }

      break;

    default:
      printf("GPU naive implementation\n");
  }

  interval_gpu<T>* d_result;
  int* d_nresults;
  int* h_nresults = new int[THREADS];
  cudaEvent_t start, stop;

  CHECKED_CALL(cudaSetDevice(devID));
  CHECKED_CALL(cudaMalloc((void**)&d_result,
                          THREADS * DEPTH_RESULT * sizeof(*d_result)));
  CHECKED_CALL(cudaMalloc((void**)&d_nresults, THREADS * sizeof(*d_nresults)));
  CHECKED_CALL(cudaEventCreate(&start));
  CHECKED_CALL(cudaEventCreate(&stop));

  if (deviceProp.major >= 2) {
    // We need L1 cache to store the stack (only applicable to sm_20 and higher)
    CHECKED_CALL(
        cudaFuncSetCacheConfig(test_interval_newton<T>, cudaFuncCachePreferL1));

    // Increase the stack size large enough for the non-inlined and recursive
    // function calls (only applicable to sm_20 and higher)
#if CUDART_VERSION >= 4000
    CHECKED_CALL(cudaDeviceSetLimit(cudaLimitStackSize, 8192));
#else
    CHECKED_CALL(cudaThreadSetLimit(cudaLimitStackSize, 8192));
#endif
  }

  interval_gpu<T> i(0.01f, 4.0f);
  std::cout << "Searching for roots in [" << i.lower() << ", " << i.upper()
            << "]...\n";

  CHECKED_CALL(cudaEventRecord(start, 0));

  for (int it = 0; it < NUM_RUNS; ++it) {
    test_interval_newton<T><<<GRID_SIZE, BLOCK_SIZE>>>(d_result, d_nresults, i,
                                                       implementation_choice);
    CHECKED_CALL(cudaGetLastError());
  }

  CHECKED_CALL(cudaEventRecord(stop, 0));
  CHECKED_CALL(cudaDeviceSynchronize());

  I_CPU* h_result = new I_CPU[THREADS * DEPTH_RESULT];
  CHECKED_CALL(cudaMemcpy(h_result, d_result,
                          THREADS * DEPTH_RESULT * sizeof(*d_result),
                          cudaMemcpyDeviceToHost));
  CHECKED_CALL(cudaMemcpy(h_nresults, d_nresults, THREADS * sizeof(*d_nresults),
                          cudaMemcpyDeviceToHost));

  std::cout << "Found " << h_nresults[0]
            << " intervals that may contain the root(s)\n";
  std::cout.precision(15);

  for (int i = 0; i != h_nresults[0]; ++i) {
    std::cout << " i[" << i << "] ="
              << " [" << h_result[THREADS * i + 0].lower() << ", "
              << h_result[THREADS * i + 0].upper() << "]\n";
  }

  float time;
  CHECKED_CALL(cudaEventElapsedTime(&time, start, stop));
  std::cout << "Number of equations solved: " << THREADS << "\n";
  std::cout << "Time per equation: "
            << 1000000.0f * (time / (float)(THREADS)) / NUM_RUNS << " us\n";

  CHECKED_CALL(cudaEventDestroy(start));
  CHECKED_CALL(cudaEventDestroy(stop));
  CHECKED_CALL(cudaFree(d_result));
  CHECKED_CALL(cudaFree(d_nresults));

  // Compute the results using a CPU implementation based on the Boost library
  I_CPU i_cpu(0.01f, 4.0f);
  I_CPU* h_result_cpu = new I_CPU[THREADS * DEPTH_RESULT];
  int* h_nresults_cpu = new int[THREADS];
  test_interval_newton_cpu<I_CPU>(h_result_cpu, h_nresults_cpu, i_cpu);

  // Compare the CPU and GPU results
  bool bTestResult =
      checkAgainstHost(h_nresults, h_nresults_cpu, h_result, h_result_cpu);

  delete[] h_result_cpu;
  delete[] h_nresults_cpu;
  delete[] h_result;
  delete[] h_nresults;

  // cudaDeviceReset causes the driver to clean up all state. While
  // not mandatory in normal operation, it is good practice.  It is also
  // needed to ensure correct operation when the application is being
  // profiled. Calling cudaDeviceReset causes all profile data to be
  // flushed before the application exits
  cudaDeviceReset();
  exit(bTestResult ? EXIT_SUCCESS : EXIT_FAILURE);
}
