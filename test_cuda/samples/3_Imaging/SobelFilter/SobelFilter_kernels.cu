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

#include <cuda_runtime.h>
#include <helper_string.h>
#include <stdio.h>
#include <stdlib.h>

#include "SobelFilter_kernels.h"

// Texture reference for reading image
texture<unsigned char, 2> tex;
extern __shared__ unsigned char LocalBlock[];
static cudaArray* array = NULL;

#define RADIUS 1

#ifdef FIXED_BLOCKWIDTH
#define BlockWidth 80
#define SharedPitch 384
#endif

// This will output the proper CUDA error strings in the event that a CUDA host
// call returns an error
#define checkCudaErrors(err) __checkCudaErrors(err, __FILE__, __LINE__)

inline void __checkCudaErrors(cudaError err, const char* file, const int line) {
  if (cudaSuccess != err) {
    fprintf(stderr, "%s(%i) : CUDA Runtime API error %d: %s.\n", file, line,
            (int)err, cudaGetErrorString(err));
    exit(EXIT_FAILURE);
  }
}

__device__ unsigned char ComputeSobel(unsigned char ul,  // upper left
                                      unsigned char um,  // upper middle
                                      unsigned char ur,  // upper right
                                      unsigned char ml,  // middle left
                                      unsigned char mm,  // middle (unused)
                                      unsigned char mr,  // middle right
                                      unsigned char ll,  // lower left
                                      unsigned char lm,  // lower middle
                                      unsigned char lr,  // lower right
                                      float fScale) {
  short Horz = ur + 2 * mr + lr - ul - 2 * ml - ll;
  short Vert = ul + 2 * um + ur - ll - 2 * lm - lr;
  short Sum = (short)(fScale * (abs((int)Horz) + abs((int)Vert)));

  if (Sum < 0) {
    return 0;
  } else if (Sum > 0xff) {
    return 0xff;
  }

  return (unsigned char)Sum;
}

__global__ void SobelShared(uchar4* pSobelOriginal, unsigned short SobelPitch,
#ifndef FIXED_BLOCKWIDTH
                            short BlockWidth, short SharedPitch,
#endif
                            short w, short h, float fScale) {
  short u = 4 * blockIdx.x * BlockWidth;
  short v = blockIdx.y * blockDim.y + threadIdx.y;
  short ib;

  int SharedIdx = threadIdx.y * SharedPitch;

  for (ib = threadIdx.x; ib < BlockWidth + 2 * RADIUS; ib += blockDim.x) {
    LocalBlock[SharedIdx + 4 * ib + 0] =
        tex2D(tex, (float)(u + 4 * ib - RADIUS + 0), (float)(v - RADIUS));
    LocalBlock[SharedIdx + 4 * ib + 1] =
        tex2D(tex, (float)(u + 4 * ib - RADIUS + 1), (float)(v - RADIUS));
    LocalBlock[SharedIdx + 4 * ib + 2] =
        tex2D(tex, (float)(u + 4 * ib - RADIUS + 2), (float)(v - RADIUS));
    LocalBlock[SharedIdx + 4 * ib + 3] =
        tex2D(tex, (float)(u + 4 * ib - RADIUS + 3), (float)(v - RADIUS));
  }

  if (threadIdx.y < RADIUS * 2) {
    //
    // copy trailing RADIUS*2 rows of pixels into shared
    //
    SharedIdx = (blockDim.y + threadIdx.y) * SharedPitch;

    for (ib = threadIdx.x; ib < BlockWidth + 2 * RADIUS; ib += blockDim.x) {
      LocalBlock[SharedIdx + 4 * ib + 0] =
          tex2D(tex, (float)(u + 4 * ib - RADIUS + 0),
                (float)(v + blockDim.y - RADIUS));
      LocalBlock[SharedIdx + 4 * ib + 1] =
          tex2D(tex, (float)(u + 4 * ib - RADIUS + 1),
                (float)(v + blockDim.y - RADIUS));
      LocalBlock[SharedIdx + 4 * ib + 2] =
          tex2D(tex, (float)(u + 4 * ib - RADIUS + 2),
                (float)(v + blockDim.y - RADIUS));
      LocalBlock[SharedIdx + 4 * ib + 3] =
          tex2D(tex, (float)(u + 4 * ib - RADIUS + 3),
                (float)(v + blockDim.y - RADIUS));
    }
  }

  __syncthreads();

  u >>= 2;  // index as uchar4 from here
  uchar4* pSobel = (uchar4*)(((char*)pSobelOriginal) + v * SobelPitch);
  SharedIdx = threadIdx.y * SharedPitch;

  for (ib = threadIdx.x; ib < BlockWidth; ib += blockDim.x) {
    unsigned char pix00 = LocalBlock[SharedIdx + 4 * ib + 0 * SharedPitch + 0];
    unsigned char pix01 = LocalBlock[SharedIdx + 4 * ib + 0 * SharedPitch + 1];
    unsigned char pix02 = LocalBlock[SharedIdx + 4 * ib + 0 * SharedPitch + 2];
    unsigned char pix10 = LocalBlock[SharedIdx + 4 * ib + 1 * SharedPitch + 0];
    unsigned char pix11 = LocalBlock[SharedIdx + 4 * ib + 1 * SharedPitch + 1];
    unsigned char pix12 = LocalBlock[SharedIdx + 4 * ib + 1 * SharedPitch + 2];
    unsigned char pix20 = LocalBlock[SharedIdx + 4 * ib + 2 * SharedPitch + 0];
    unsigned char pix21 = LocalBlock[SharedIdx + 4 * ib + 2 * SharedPitch + 1];
    unsigned char pix22 = LocalBlock[SharedIdx + 4 * ib + 2 * SharedPitch + 2];

    uchar4 out;

    out.x = ComputeSobel(pix00, pix01, pix02, pix10, pix11, pix12, pix20, pix21,
                         pix22, fScale);

    pix00 = LocalBlock[SharedIdx + 4 * ib + 0 * SharedPitch + 3];
    pix10 = LocalBlock[SharedIdx + 4 * ib + 1 * SharedPitch + 3];
    pix20 = LocalBlock[SharedIdx + 4 * ib + 2 * SharedPitch + 3];
    out.y = ComputeSobel(pix01, pix02, pix00, pix11, pix12, pix10, pix21, pix22,
                         pix20, fScale);

    pix01 = LocalBlock[SharedIdx + 4 * ib + 0 * SharedPitch + 4];
    pix11 = LocalBlock[SharedIdx + 4 * ib + 1 * SharedPitch + 4];
    pix21 = LocalBlock[SharedIdx + 4 * ib + 2 * SharedPitch + 4];
    out.z = ComputeSobel(pix02, pix00, pix01, pix12, pix10, pix11, pix22, pix20,
                         pix21, fScale);

    pix02 = LocalBlock[SharedIdx + 4 * ib + 0 * SharedPitch + 5];
    pix12 = LocalBlock[SharedIdx + 4 * ib + 1 * SharedPitch + 5];
    pix22 = LocalBlock[SharedIdx + 4 * ib + 2 * SharedPitch + 5];
    out.w = ComputeSobel(pix00, pix01, pix02, pix10, pix11, pix12, pix20, pix21,
                         pix22, fScale);

    if (u + ib < w / 4 && v < h) {
      pSobel[u + ib] = out;
    }
  }

  __syncthreads();
}

__global__ void SobelCopyImage(Pixel* pSobelOriginal, unsigned int Pitch, int w,
                               int h, float fscale) {
  unsigned char* pSobel =
      (unsigned char*)(((char*)pSobelOriginal) + blockIdx.x * Pitch);

  for (int i = threadIdx.x; i < w; i += blockDim.x) {
    pSobel[i] = min(
        max((tex2D(tex, (float)i, (float)blockIdx.x) * fscale), 0.f), 255.f);
  }
}

__global__ void SobelTex(Pixel* pSobelOriginal, unsigned int Pitch, int w,
                         int h, float fScale) {
  unsigned char* pSobel =
      (unsigned char*)(((char*)pSobelOriginal) + blockIdx.x * Pitch);

  for (int i = threadIdx.x; i < w; i += blockDim.x) {
    unsigned char pix00 = tex2D(tex, (float)i - 1, (float)blockIdx.x - 1);
    unsigned char pix01 = tex2D(tex, (float)i + 0, (float)blockIdx.x - 1);
    unsigned char pix02 = tex2D(tex, (float)i + 1, (float)blockIdx.x - 1);
    unsigned char pix10 = tex2D(tex, (float)i - 1, (float)blockIdx.x + 0);
    unsigned char pix11 = tex2D(tex, (float)i + 0, (float)blockIdx.x + 0);
    unsigned char pix12 = tex2D(tex, (float)i + 1, (float)blockIdx.x + 0);
    unsigned char pix20 = tex2D(tex, (float)i - 1, (float)blockIdx.x + 1);
    unsigned char pix21 = tex2D(tex, (float)i + 0, (float)blockIdx.x + 1);
    unsigned char pix22 = tex2D(tex, (float)i + 1, (float)blockIdx.x + 1);
    pSobel[i] = ComputeSobel(pix00, pix01, pix02, pix10, pix11, pix12, pix20,
                             pix21, pix22, fScale);
  }
}

extern "C" void setupTexture(int iw, int ih, Pixel* data, int Bpp) {
  cudaChannelFormatDesc desc;

  if (Bpp == 1) {
    desc = cudaCreateChannelDesc<unsigned char>();
  } else {
    desc = cudaCreateChannelDesc<uchar4>();
  }

  checkCudaErrors(cudaMallocArray(&array, &desc, iw, ih));
  checkCudaErrors(cudaMemcpyToArray(array, 0, 0, data,
                                    Bpp * sizeof(Pixel) * iw * ih,
                                    cudaMemcpyHostToDevice));
}

extern "C" void deleteTexture(void) { checkCudaErrors(cudaFreeArray(array)); }

// Wrapper for the __global__ call that sets up the texture and threads
extern "C" void sobelFilter(Pixel* odata, int iw, int ih,
                            enum SobelDisplayMode mode, float fScale) {
  checkCudaErrors(cudaBindTextureToArray(tex, array));

  switch (mode) {
    case SOBELDISPLAY_IMAGE:
      SobelCopyImage<<<ih, 384>>>(odata, iw, iw, ih, fScale);
      break;

    case SOBELDISPLAY_SOBELTEX:
      SobelTex<<<ih, 384>>>(odata, iw, iw, ih, fScale);
      break;

    case SOBELDISPLAY_SOBELSHARED: {
      dim3 threads(16, 4);
#ifndef FIXED_BLOCKWIDTH
      int BlockWidth = 80;  // must be divisible by 16 for coalescing
#endif
      dim3 blocks = dim3(iw / (4 * BlockWidth) + (0 != iw % (4 * BlockWidth)),
                         ih / threads.y + (0 != ih % threads.y));
      int SharedPitch = ~0x3f & (4 * (BlockWidth + 2 * RADIUS) + 0x3f);
      int sharedMem = SharedPitch * (threads.y + 2 * RADIUS);

      // for the shared kernel, width must be divisible by 4
      iw &= ~3;

      SobelShared<<<blocks, threads, sharedMem>>>((uchar4*)odata, iw,
#ifndef FIXED_BLOCKWIDTH
                                                  BlockWidth, SharedPitch,
#endif
                                                  iw, ih, fScale);
    } break;
  }

  checkCudaErrors(cudaUnbindTexture(tex));
}
