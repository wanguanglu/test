/**
 * Copyright 1993-2015 NVIDIA Corporation.  All rights reserved.
 *
 * Please refer to the NVIDIA end user license agreement (EULA) associated
 * with this source code for terms and conditions that govern your use of
 * this software. Any use, reproduction, disclosure, or distribution of
 * this software and related documentation outside the terms of the EULA
 * is strictly prohibited.
 *
 */

////////////////////////////////////////////////////////////////////////////////
//
//  simpleCUFFT_2d_MGPU.cu
//
//  This sample code demonstrate the use of CUFFT library for 2D data on
//  multiple GPU. Example showing the use of CUFFT for solving 2D-POISSON
//  equation using FFT on multiple GPU. For reference we have used the equation
//  given in http://www.bu.edu/pasi/files/2011/07/ Lecture83.pdf
//
////////////////////////////////////////////////////////////////////////////////

// System includes
#include <stdio.h>
#include <stdlib.h>

#include <math.h>
#include <string.h>

// CUDA runtime
#include <cuda_runtime.h>

// CUFFT Header file
#include <cufftXt.h>

// helper functions and utilities to work with CUDA
#include <helper_cuda.h>
#include <helper_functions.h>

// Complex data type
typedef float2 Complex;

// Data configuration
const int GPU_COUNT = 2;
const int BSZ_Y = 4;
const int BSZ_X = 4;

// Forward Declaration
void solvePoissonEquation(cudaLibXtDesc*, cudaLibXtDesc*, float**, int, int);

__global__ void solvePoisson(cufftComplex*, cufftComplex*, float*, int, int,
                             int n_gpu);

///////////////////////////////////////////////////////////////////////////////
// Program main
////////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv) {
  printf(
      "\nPoisson equation using CUFFT library on Multiple GPUs is "
      "starting...\n\n");

  int GPU_N;
  checkCudaErrors(cudaGetDeviceCount(&GPU_N));

  if (GPU_N < GPU_COUNT) {
    printf("No. of GPU on node %d\n", GPU_N);
    printf("Two GPUs are required to run simpleCUFFT_2d_MGPU sample code\n");
    exit(EXIT_WAIVED);
  }

  int N = 64;
  float xMAX = 1.0f, xMIN = 0.0f, yMIN = 0.0f, h = (xMAX - xMIN) / ((float)N),
        s = 0.1, s2 = s * s;
  float *x, *y, *f, *u_a, r2;

  x = (float*)malloc(sizeof(float) * N * N);
  y = (float*)malloc(sizeof(float) * N * N);
  f = (float*)malloc(sizeof(float) * N * N);
  u_a = (float*)malloc(sizeof(float) * N * N);

  for (int j = 0; j < N; j++)
    for (int i = 0; i < N; i++) {
      x[N * j + i] = xMIN + i * h;
      y[N * j + i] = yMIN + j * h;
      r2 = (x[N * j + i] - 0.5) * (x[N * j + i] - 0.5) +
           (y[N * j + i] - 0.5) * (y[N * j + i] - 0.5);
      f[N * j + i] = (r2 - 2 * s2) / (s2 * s2) * exp(-r2 / (2 * s2));
      u_a[N * j + i] = exp(-r2 / (2 * s2));  // analytical solution
    }

  float *k, *d_k[GPU_COUNT];
  k = (float*)malloc(sizeof(float) * N);
  for (int i = 0; i <= N / 2; i++) {
    k[i] = i * 2 * M_PI;
  }
  for (int i = N / 2 + 1; i < N; i++) {
    k[i] = (i - N) * 2 * M_PI;
  }

  // Create a complex variable on host
  Complex* h_f = (Complex*)malloc(sizeof(Complex) * N * N);

  // Initialize the memory for the signal
  for (int i = 0; i < (N * N); i++) {
    h_f[i].x = f[i];
    h_f[i].y = 0.0f;
  }

  // cufftCreate() - Create an empty plan
  cufftResult result;
  cufftHandle planComplex;
  result = cufftCreate(&planComplex);
  if (result != CUFFT_SUCCESS) {
    printf("cufftCreate failed\n");
    exit(EXIT_FAILURE);
  }

  // cufftXtSetGPUs() - Define which GPUs to use
  int nGPUs = 2;
  int* whichGPUs;
  whichGPUs = (int*)malloc(sizeof(int) * nGPUs);

  // Iterate all device combinations to see if a supported combo exists
  for (int i = 0; i < GPU_N; i++) {
    for (int j = i + 1; j < GPU_N; j++) {
      whichGPUs[0] = i;
      whichGPUs[1] = j;
      result = cufftXtSetGPUs(planComplex, nGPUs, whichGPUs);

      if (result == CUFFT_INVALID_DEVICE) {
        continue;
      } else if (result == CUFFT_SUCCESS) {
        break;
      } else {
        printf("cufftXtSetGPUs failed\n");
        exit(EXIT_FAILURE);
      }
    }

    if (result == CUFFT_SUCCESS) {
      break;
    }
  }

  if (result == CUFFT_INVALID_DEVICE) {
    printf("This sample requires two GPUs on the same board.\n");
    printf("No such board was found. Waiving sample.\n");
    exit(EXIT_WAIVED);
  }

  // Print the device information to run the code
  for (int i = 0; i < 2; i++) {
    cudaDeviceProp deviceProp;
    checkCudaErrors(cudaGetDeviceProperties(&deviceProp, whichGPUs[i]));
    printf("GPU Device %d: \"%s\" with compute capability %d.%d\n",
           whichGPUs[i], deviceProp.name, deviceProp.major, deviceProp.minor);
  }

  result = cufftXtSetGPUs(planComplex, nGPUs, whichGPUs);
  if (result != CUFFT_SUCCESS) {
    printf("cufftXtSetGPUs failed\n");
    exit(EXIT_FAILURE);
  }

  size_t* worksize;
  worksize = (size_t*)malloc(sizeof(size_t) * nGPUs);

  // cufftMakePlan2d() - Create the plan
  result = cufftMakePlan2d(planComplex, N, N, CUFFT_C2C, worksize);
  if (result != CUFFT_SUCCESS) {
    printf("*MakePlan* failed\n");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < nGPUs; i++) {
    cudaSetDevice(whichGPUs[i]);
    cudaMalloc((void**)&d_k[i], sizeof(float) * N);
    cudaMemcpy(d_k[i], k, sizeof(float) * N, cudaMemcpyHostToDevice);
  }

  // Create a variable on device
  // d_f - variable on device to store the input data
  // d_d_f - variable that store the natural order of d_f data
  // d_out - device output
  cudaLibXtDesc *d_f, *d_d_f, *d_out;

  // cufftXtMalloc() - Malloc data on multiple GPUs

  result = cufftXtMalloc(planComplex, (cudaLibXtDesc**)&d_f,
                         CUFFT_XT_FORMAT_INPLACE);
  if (result != CUFFT_SUCCESS) {
    printf("*XtMalloc failed\n");
    exit(EXIT_FAILURE);
  }

  result = cufftXtMalloc(planComplex, (cudaLibXtDesc**)&d_d_f,
                         CUFFT_XT_FORMAT_INPLACE);
  if (result != CUFFT_SUCCESS) {
    printf("*XtMalloc failed\n");
    exit(EXIT_FAILURE);
  }

  result = cufftXtMalloc(planComplex, (cudaLibXtDesc**)&d_out,
                         CUFFT_XT_FORMAT_INPLACE);
  if (result != CUFFT_SUCCESS) {
    printf("*XtMalloc failed\n");
    exit(EXIT_FAILURE);
  }

  // cufftXtMemcpy() - Copy the data from host to device
  result = cufftXtMemcpy(planComplex, d_f, h_f, CUFFT_COPY_HOST_TO_DEVICE);
  if (result != CUFFT_SUCCESS) {
    printf("*XtMemcpy failed\n");
    exit(EXIT_FAILURE);
  }

  // cufftXtExecDescriptorC2C() - Execute FFT on data on multiple GPUs
  printf("Forward 2d FFT on multiple GPUs\n");
  result = cufftXtExecDescriptorC2C(planComplex, d_f, d_f, CUFFT_FORWARD);
  if (result != CUFFT_SUCCESS) {
    printf("*XtExecC2C  failed\n");
    exit(EXIT_FAILURE);
  }

  // cufftXtMemcpy() - Copy the data to natural order on GPUs
  result = cufftXtMemcpy(planComplex, d_d_f, d_f, CUFFT_COPY_DEVICE_TO_DEVICE);
  if (result != CUFFT_SUCCESS) {
    printf("*XtMemcpy failed\n");
    exit(EXIT_FAILURE);
  }

  printf("Solve Poisson Equation\n");
  solvePoissonEquation(d_d_f, d_out, d_k, N, nGPUs);

  printf("Inverse 2d FFT on multiple GPUs\n");
  // cufftXtExecDescriptorC2C() - Execute inverse  FFT on data on multiple GPUs
  result = cufftXtExecDescriptorC2C(planComplex, d_out, d_out, CUFFT_INVERSE);
  if (result != CUFFT_SUCCESS) {
    printf("*XtExecC2C  failed\n");
    exit(EXIT_FAILURE);
  }

  // Create a variable on host to copy the data from device
  // h_d_out - variable store the output of device
  Complex* h_d_out = (Complex*)malloc(sizeof(Complex) * N * N);

  // cufftXtMemcpy() - Copy data from multiple GPUs to host
  result =
      cufftXtMemcpy(planComplex, h_d_out, d_out, CUFFT_COPY_DEVICE_TO_HOST);
  if (result != CUFFT_SUCCESS) {
    printf("*XtMemcpy failed\n");
    exit(EXIT_FAILURE);
  }

  float* out = (float*)malloc(sizeof(float) * N * N);
  float constant = h_d_out[0].x / N * N;
  for (int i = 0; i < N * N; i++) {
    // subtract u[0] to force the arbitrary constant to be 0
    out[i] = (h_d_out[i].x / (N * N)) - constant;
  }

  // cleanup memory

  free(h_f);
  free(k);
  free(out);
  free(h_d_out);
  free(x);
  free(whichGPUs);
  free(y);
  free(f);
  free(u_a);
  free(worksize);

  // cudaXtFree() - Free GPU memory
  for (int i = 0; i < GPU_COUNT; i++) {
    cudaFree(d_k[i]);
  }
  result = cufftXtFree(d_out);
  if (result != CUFFT_SUCCESS) {
    printf("*XtFree failed\n");
    exit(EXIT_FAILURE);
  }
  result = cufftXtFree(d_f);
  if (result != CUFFT_SUCCESS) {
    printf("*XtFree failed\n");
    exit(EXIT_FAILURE);
  }
  result = cufftXtFree(d_d_f);
  if (result != CUFFT_SUCCESS) {
    printf("*XtFree failed\n");
    exit(EXIT_FAILURE);
  }

  // cufftDestroy() - Destroy FFT plan
  result = cufftDestroy(planComplex);
  if (result != CUFFT_SUCCESS) {
    printf("cufftDestroy failed: code %d\n", (int)result);
    exit(EXIT_FAILURE);
  }

  cudaDeviceReset();
  exit(EXIT_SUCCESS);
}

////////////////////////////////////////////////////////////////////////////////////
// Launch kernel on  multiple GPU
///////////////////////////////////////////////////////////////////////////////////
void solvePoissonEquation(cudaLibXtDesc* d_ft, cudaLibXtDesc* d_ft_k, float** k,
                          int N, int nGPUs) {
  int device;
  dim3 dimGrid(int(N / BSZ_X), int((N / 2) / BSZ_Y));
  dim3 dimBlock(BSZ_X, BSZ_Y);

  for (int i = 0; i < nGPUs; i++) {
    device = d_ft_k->descriptor->GPUs[i];
    cudaSetDevice(device);
    solvePoisson<<<dimGrid, dimBlock>>>(
        (cufftComplex*)d_ft->descriptor->data[i],
        (cufftComplex*)d_ft_k->descriptor->data[i], k[i], N, i, nGPUs);
  }

  // Wait for device to finish all operation
  for (int i = 0; i < nGPUs; i++) {
    device = d_ft_k->descriptor->GPUs[i];
    cudaSetDevice(device);
    cudaDeviceSynchronize();

    // Check if kernel execution generated and error
    getLastCudaError("Kernel execution failed [ solvePoisson ]");
  }
}

////////////////////////////////////////////////////////////////////////////////
// Kernel for Solving Poisson equation on GPU
////////////////////////////////////////////////////////////////////////////////
__global__ void solvePoisson(cufftComplex* ft, cufftComplex* ft_k, float* k,
                             int N, int gpu_id, int n_gpu) {
  int i = threadIdx.x + blockIdx.x * blockDim.x;
  int j = threadIdx.y + blockIdx.y * blockDim.y;
  int index = j * N + i;
  if (i < N && j < N / n_gpu) {
    float k2 =
        k[i] * k[i] + k[j + gpu_id * N / n_gpu] * k[j + gpu_id * N / n_gpu];
    if (i == 0 && j == 0 && gpu_id == 0) {
      k2 = 1.0f;
    }

    ft_k[index].x = -ft[index].x * 1 / k2;
    ft_k[index].y = -ft[index].y * 1 / k2;
  }
}
