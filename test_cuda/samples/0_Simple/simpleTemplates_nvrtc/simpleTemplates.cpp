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

/* This sample is a templatized version of the template project.
 * It also shows how to correctly templatize dynamically allocated shared
 * memory arrays.
 * Host code.
 */

// System includes
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

// CUDA runtime
#include <cuda_runtime.h>

// helper functions and utilities to work with CUDA
#include <helper_functions.h>
#include <nvrtc_helper.h>
#include <timer.h>

#include <cudaProfiler.h>

#ifndef MAX
#define MAX(a, b) (a > b ? a : b)
#endif

int g_TotalFailures = 0;

////////////////////////////////////////////////////////////////////////////////
// declaration, forward

template <class T>
void runTest(int argc, char** argv, int len);

template <class T>
void computeGold(T* reference, T* idata, const unsigned int len) {
  const T T_len = static_cast<T>(len);

  for (unsigned int i = 0; i < len; ++i) {
    reference[i] = idata[i] * T_len;
  }
}

////////////////////////////////////////////////////////////////////////////////
// Program main
////////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {
  printf("> runTest<float,32>\n");

  runTest<float>(argc, argv, 32);

  printf("> runTest<int,64>\n");

  runTest<int>(argc, argv, 64);

  printf("\n[simpleTemplates_nvrtc] -> Test Results: %d Failures\n",
         g_TotalFailures);

  cuProfilerStop();

  exit(g_TotalFailures == 0 ? EXIT_SUCCESS : EXIT_FAILURE);
}

// To completely templatize runTest (below) with cutil, we need to use
// template specialization to wrap up CUTIL's array comparison and file writing
// functions for different types.

// Here's the generic wrapper for cutCompare*
template <class T>
class ArrayComparator {
 public:
  bool compare(const T* reference, T* data, unsigned int len) {
    fprintf(stderr,
            "Error: no comparison function implemented for this type\n");
    return false;
  }
};

// Here's the specialization for ints:
template <>
class ArrayComparator<int> {
 public:
  bool compare(const int* reference, int* data, unsigned int len) {
    return compareData(reference, data, len, 0.15f, 0.0f);
  }
};

// Here's the specialization for floats:
template <>
class ArrayComparator<float> {
 public:
  bool compare(const float* reference, float* data, unsigned int len) {
    return compareData(reference, data, len, 0.15f, 0.15f);
  }
};

// Here's the generic wrapper for cutWriteFile*
template <class T>
class ArrayFileWriter {
 public:
  bool write(const char* filename, T* data, unsigned int len, float epsilon) {
    fprintf(stderr,
            "Error: no file write function implemented for this type\n");
    return false;
  }
};

// Here's the specialization for ints:
template <>
class ArrayFileWriter<int> {
 public:
  bool write(const char* filename, int* data, unsigned int len, float epsilon) {
    return sdkWriteFile(filename, data, len, epsilon, false);
  }
};

// Here's the specialization for floats:
template <>
class ArrayFileWriter<float> {
 public:
  bool write(const char* filename, float* data, unsigned int len,
             float epsilon) {
    return sdkWriteFile(filename, data, len, epsilon, false);
  }
};

template <typename T>
CUfunction getKernel(CUmodule in);

template <>
CUfunction getKernel<int>(CUmodule in) {
  CUfunction kernel_addr;
  checkCudaErrors(cuModuleGetFunction(&kernel_addr, in, "testInt"));

  return kernel_addr;
}

template <>
CUfunction getKernel<float>(CUmodule in) {
  CUfunction kernel_addr;
  checkCudaErrors(cuModuleGetFunction(&kernel_addr, in, "testFloat"));

  return kernel_addr;
}

////////////////////////////////////////////////////////////////////////////////
//! Run a simple test for CUDA
////////////////////////////////////////////////////////////////////////////////

static bool moduleLoaded = false;
CUmodule module;
char *ptx, *kernel_file;
size_t ptxSize;

template <class T>
void runTest(int argc, char** argv, int len) {
  if (!moduleLoaded) {
    kernel_file = sdkFindFilePath("simpleTemplates_kernel.cu", argv[0]);
    compileFileToPTX(kernel_file, 0, NULL, &ptx, &ptxSize);
    module = loadPTX(ptx, argc, argv);
    moduleLoaded = true;
  }

  StartTimer();

  unsigned int num_threads = len;
  unsigned int mem_size = sizeof(float) * num_threads;

  // allocate host memory
  T* h_idata = (T*)malloc(mem_size);

  // initialize the memory
  for (unsigned int i = 0; i < num_threads; ++i) {
    h_idata[i] = (T)i;
  }

  // allocate device memory
  CUdeviceptr d_idata;
  checkCudaErrors(cuMemAlloc(&d_idata, mem_size));

  // copy host memory to device
  checkCudaErrors(cuMemcpyHtoD(d_idata, h_idata, mem_size));

  // allocate device memory for result
  CUdeviceptr d_odata;
  checkCudaErrors(cuMemAlloc(&d_odata, mem_size));

  // setup execution parameters
  dim3 grid(1, 1, 1);
  dim3 threads(num_threads, 1, 1);

  // execute the kernel
  CUfunction kernel_addr = getKernel<T>(module);

  void* arr[] = {(void*)&d_idata, (void*)&d_odata};
  checkCudaErrors(
      cuLaunchKernel(kernel_addr, grid.x, grid.y, grid.z, /* grid dim */
                     threads.x, threads.y, threads.z,     /* block dim */
                     mem_size, 0, /* shared mem, stream */
                     &arr[0],     /* arguments */
                     0));

  // check if kernel execution generated and error
  checkCudaErrors(cuCtxSynchronize());

  // allocate mem for the result on host side
  T* h_odata = (T*)malloc(mem_size);

  // copy result from device to host
  checkCudaErrors(cuMemcpyDtoH(h_odata, d_odata, sizeof(T) * num_threads));

  printf("Processing time: %f (ms)\n", GetTimer());

  // compute reference solution
  T* reference = (T*)malloc(mem_size);

  computeGold<T>(reference, h_idata, num_threads);

  ArrayComparator<T> comparator;
  ArrayFileWriter<T> writer;

  // check result
  if (checkCmdLineFlag(argc, (const char**)argv, "regression")) {
    // write file for regression test
    writer.write("./data/regression.dat", h_odata, num_threads, 0.0f);
  } else {
    // custom output handling when no regression test running
    // in this case check if the result is equivalent to the expected solution
    bool res = comparator.compare(reference, h_odata, num_threads);

    printf("Compare %s\n\n", (1 == res) ? "OK" : "MISMATCH");

    g_TotalFailures += (1 != res);
  }

  // cleanup memory
  free(h_idata);
  free(h_odata);
  free(reference);
  checkCudaErrors(cuMemFree(d_idata));
  checkCudaErrors(cuMemFree(d_odata));
}
