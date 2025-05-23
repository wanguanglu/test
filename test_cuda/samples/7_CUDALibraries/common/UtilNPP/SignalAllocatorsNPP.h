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

#ifndef NV_UTIL_NPP_SIGNAL_ALLOCATORS_NPP_H
#define NV_UTIL_NPP_SIGNAL_ALLOCATORS_NPP_H

#include "Exceptions.h"

#include <cuda_runtime.h>
#include <npps.h>

namespace npp {

template <typename D>
class SignalAllocator {};

template <>
class SignalAllocator<Npp8u> {
 public:
  static Npp8u* Malloc1D(size_t nSize) {
    Npp8u* pResult = nppsMalloc_8u(static_cast<int>(nSize));
    NPP_ASSERT(pResult != 0);

    return pResult;
  };

  static void Free1D(Npp8u* pValues) { nppsFree(pValues); };

  static void Copy1D(Npp8u* pDst, const Npp8u* pSrc, size_t nSize) {
    cudaError_t eResult;
    eResult =
        cudaMemcpy(pDst, pSrc, nSize * sizeof(Npp8u), cudaMemcpyDeviceToDevice);
    NPP_ASSERT(cudaSuccess == eResult);
  };

  static void HostToDeviceCopy1D(Npp8u* pDst, const Npp8u* pSrc, size_t nSize) {
    cudaError_t eResult;
    eResult =
        cudaMemcpy(pDst, pSrc, nSize * sizeof(Npp8u), cudaMemcpyHostToDevice);
    NPP_ASSERT(cudaSuccess == eResult);
  };

  static void DeviceToHostCopy1D(Npp8u* pDst, const Npp8u* pSrc, size_t nSize) {
    cudaError_t eResult;
    eResult =
        cudaMemcpy(pDst, pSrc, nSize * sizeof(Npp8u), cudaMemcpyDeviceToHost);
    NPP_ASSERT(cudaSuccess == eResult);
  };
};

template <>
class SignalAllocator<Npp16s> {
 public:
  static Npp16s* Malloc1D(size_t nSize) {
    Npp16s* pResult = nppsMalloc_16s(static_cast<int>(nSize));
    NPP_ASSERT(pResult != 0);

    return pResult;
  };

  static void Free1D(Npp16s* pValues) { nppsFree(pValues); };

  static void Copy1D(Npp16s* pDst, const Npp16s* pSrc, size_t nSize) {
    cudaError_t eResult;
    eResult = cudaMemcpy(pDst, pSrc, nSize * sizeof(Npp16s),
                         cudaMemcpyDeviceToDevice);
    NPP_ASSERT(cudaSuccess == eResult);
  };

  static void HostToDeviceCopy1D(Npp16s* pDst, const Npp16s* pSrc,
                                 size_t nSize) {
    cudaError_t eResult;
    eResult =
        cudaMemcpy(pDst, pSrc, nSize * sizeof(Npp16s), cudaMemcpyHostToDevice);
    NPP_ASSERT(cudaSuccess == eResult);
  };

  static void DeviceToHostCopy1D(Npp16s* pDst, const Npp16s* pSrc,
                                 size_t nSize) {
    cudaError_t eResult;
    eResult =
        cudaMemcpy(pDst, pSrc, nSize * sizeof(Npp16s), cudaMemcpyDeviceToHost);
    NPP_ASSERT(cudaSuccess == eResult);
  };
};

template <>
class SignalAllocator<Npp16u> {
 public:
  static Npp16u* Malloc1D(size_t nSize) {
    Npp16u* pResult = nppsMalloc_16u(static_cast<int>(nSize));
    NPP_ASSERT(pResult != 0);

    return pResult;
  };

  static void Free1D(Npp16u* pValues) { nppsFree(pValues); };

  static void Copy1D(Npp16u* pDst, const Npp16u* pSrc, size_t nSize) {
    cudaError_t eResult;
    eResult = cudaMemcpy(pDst, pSrc, nSize * sizeof(Npp16u),
                         cudaMemcpyDeviceToDevice);
    NPP_ASSERT(cudaSuccess == eResult);
  };

  static void HostToDeviceCopy1D(Npp16u* pDst, const Npp16u* pSrc,
                                 size_t nSize) {
    cudaError_t eResult;
    eResult =
        cudaMemcpy(pDst, pSrc, nSize * sizeof(Npp16u), cudaMemcpyHostToDevice);
    NPP_ASSERT(cudaSuccess == eResult);
  };

  static void DeviceToHostCopy1D(Npp16u* pDst, const Npp16u* pSrc,
                                 size_t nSize) {
    cudaError_t eResult;
    eResult =
        cudaMemcpy(pDst, pSrc, nSize * sizeof(Npp16u), cudaMemcpyDeviceToHost);
    NPP_ASSERT(cudaSuccess == eResult);
  };
};

template <>
class SignalAllocator<Npp16sc> {
 public:
  static Npp16sc* Malloc1D(size_t nSize) {
    Npp16sc* pResult = nppsMalloc_16sc(static_cast<int>(nSize));
    NPP_ASSERT(pResult != 0);

    return pResult;
  };

  static void Free1D(Npp16sc* pValues) { nppsFree(pValues); };

  static void Copy1D(Npp16sc* pDst, const Npp16sc* pSrc, size_t nSize) {
    cudaError_t eResult;
    eResult = cudaMemcpy(pDst, pSrc, nSize * sizeof(Npp16sc),
                         cudaMemcpyDeviceToDevice);
    NPP_ASSERT(cudaSuccess == eResult);
  };

  static void HostToDeviceCopy1D(Npp16sc* pDst, const Npp16sc* pSrc,
                                 size_t nSize) {
    cudaError_t eResult;
    eResult =
        cudaMemcpy(pDst, pSrc, nSize * sizeof(Npp16sc), cudaMemcpyHostToDevice);
    NPP_ASSERT(cudaSuccess == eResult);
  };

  static void DeviceToHostCopy1D(Npp16sc* pDst, const Npp16sc* pSrc,
                                 size_t nSize) {
    cudaError_t eResult;
    eResult =
        cudaMemcpy(pDst, pSrc, nSize * sizeof(Npp16sc), cudaMemcpyDeviceToHost);
    NPP_ASSERT(cudaSuccess == eResult);
  };
};

template <>
class SignalAllocator<Npp32u> {
 public:
  static Npp32u* Malloc1D(size_t nSize) {
    Npp32u* pResult = nppsMalloc_32u(static_cast<int>(nSize));
    NPP_ASSERT(pResult != 0);

    return pResult;
  };

  static void Free1D(Npp32u* pValues) { nppsFree(pValues); };

  static void Copy1D(Npp32u* pDst, const Npp32u* pSrc, size_t nSize) {
    cudaError_t eResult;
    eResult = cudaMemcpy(pDst, pSrc, nSize * sizeof(Npp32u),
                         cudaMemcpyDeviceToDevice);
    NPP_ASSERT(cudaSuccess == eResult);
  };

  static void HostToDeviceCopy1D(Npp32u* pDst, const Npp32u* pSrc,
                                 size_t nSize) {
    cudaError_t eResult;
    eResult =
        cudaMemcpy(pDst, pSrc, nSize * sizeof(Npp32u), cudaMemcpyHostToDevice);
    NPP_ASSERT(cudaSuccess == eResult);
  };

  static void DeviceToHostCopy1D(Npp32u* pDst, const Npp32u* pSrc,
                                 size_t nSize) {
    cudaError_t eResult;
    eResult =
        cudaMemcpy(pDst, pSrc, nSize * sizeof(Npp32u), cudaMemcpyDeviceToHost);
    NPP_ASSERT(cudaSuccess == eResult);
  };
};

template <>
class SignalAllocator<Npp32s> {
 public:
  static Npp32s* Malloc1D(size_t nSize) {
    Npp32s* pResult = nppsMalloc_32s(static_cast<int>(nSize));
    NPP_ASSERT(pResult != 0);

    return pResult;
  };

  static void Free1D(Npp32s* pValues) { nppsFree(pValues); };

  static void Copy1D(Npp32s* pDst, const Npp32s* pSrc, size_t nSize) {
    cudaError_t eResult;
    eResult = cudaMemcpy(pDst, pSrc, nSize * sizeof(Npp32s),
                         cudaMemcpyDeviceToDevice);
    NPP_ASSERT(cudaSuccess == eResult);
  };

  static void HostToDeviceCopy1D(Npp32s* pDst, const Npp32s* pSrc,
                                 size_t nSize) {
    cudaError_t eResult;
    eResult =
        cudaMemcpy(pDst, pSrc, nSize * sizeof(Npp32s), cudaMemcpyHostToDevice);
    NPP_ASSERT(cudaSuccess == eResult);
  };

  static void DeviceToHostCopy1D(Npp32s* pDst, const Npp32s* pSrc,
                                 size_t nSize) {
    cudaError_t eResult;
    eResult =
        cudaMemcpy(pDst, pSrc, nSize * sizeof(Npp32s), cudaMemcpyDeviceToHost);
    NPP_ASSERT(cudaSuccess == eResult);
  };
};

template <>
class SignalAllocator<Npp32sc> {
 public:
  static Npp32sc* Malloc1D(size_t nSize) {
    Npp32sc* pResult = nppsMalloc_32sc(static_cast<int>(nSize));
    NPP_ASSERT(pResult != 0);

    return pResult;
  };

  static void Free1D(Npp32sc* pValues) { nppsFree(pValues); };

  static void Copy1D(Npp32sc* pDst, const Npp32sc* pSrc, size_t nSize) {
    cudaError_t eResult;
    eResult = cudaMemcpy(pDst, pSrc, nSize * sizeof(Npp32sc),
                         cudaMemcpyDeviceToDevice);
    NPP_ASSERT(cudaSuccess == eResult);
  };

  static void HostToDeviceCopy1D(Npp32sc* pDst, const Npp32sc* pSrc,
                                 size_t nSize) {
    cudaError_t eResult;
    eResult =
        cudaMemcpy(pDst, pSrc, nSize * sizeof(Npp32sc), cudaMemcpyHostToDevice);
    NPP_ASSERT(cudaSuccess == eResult);
  };

  static void DeviceToHostCopy1D(Npp32sc* pDst, const Npp32sc* pSrc,
                                 size_t nSize) {
    cudaError_t eResult;
    eResult =
        cudaMemcpy(pDst, pSrc, nSize * sizeof(Npp32sc), cudaMemcpyDeviceToHost);
    NPP_ASSERT(cudaSuccess == eResult);
  };
};

template <>
class SignalAllocator<Npp32f> {
 public:
  static Npp32f* Malloc1D(size_t nSize) {
    Npp32f* pResult = nppsMalloc_32f(static_cast<int>(nSize));
    NPP_ASSERT(pResult != 0);

    return pResult;
  };

  static void Free1D(Npp32f* pValues) { nppsFree(pValues); };

  static void Copy1D(Npp32f* pDst, const Npp32f* pSrc, size_t nSize) {
    cudaError_t eResult;
    eResult = cudaMemcpy(pDst, pSrc, nSize * sizeof(Npp32f),
                         cudaMemcpyDeviceToDevice);
    NPP_ASSERT(cudaSuccess == eResult);
  };

  static void HostToDeviceCopy1D(Npp32f* pDst, const Npp32f* pSrc,
                                 size_t nSize) {
    cudaError_t eResult;
    eResult =
        cudaMemcpy(pDst, pSrc, nSize * sizeof(Npp32f), cudaMemcpyHostToDevice);
    NPP_ASSERT(cudaSuccess == eResult);
  };

  static void DeviceToHostCopy1D(Npp32f* pDst, const Npp32f* pSrc,
                                 size_t nSize) {
    cudaError_t eResult;
    eResult =
        cudaMemcpy(pDst, pSrc, nSize * sizeof(Npp32f), cudaMemcpyDeviceToHost);
    NPP_ASSERT(cudaSuccess == eResult);
  };
};

template <>
class SignalAllocator<Npp32fc> {
 public:
  static Npp32fc* Malloc1D(size_t nSize) {
    Npp32fc* pResult = nppsMalloc_32fc(static_cast<int>(nSize));
    NPP_ASSERT(pResult != 0);

    return pResult;
  };

  static void Free1D(Npp32fc* pValues) { nppsFree(pValues); };

  static void Copy1D(Npp32fc* pDst, const Npp32fc* pSrc, size_t nSize) {
    cudaError_t eResult;
    eResult = cudaMemcpy(pDst, pSrc, nSize * sizeof(Npp32fc),
                         cudaMemcpyDeviceToDevice);
    NPP_ASSERT(cudaSuccess == eResult);
  };

  static void HostToDeviceCopy1D(Npp32fc* pDst, const Npp32fc* pSrc,
                                 size_t nSize) {
    cudaError_t eResult;
    eResult =
        cudaMemcpy(pDst, pSrc, nSize * sizeof(Npp32fc), cudaMemcpyHostToDevice);
    NPP_ASSERT(cudaSuccess == eResult);
  };

  static void DeviceToHostCopy1D(Npp32fc* pDst, const Npp32fc* pSrc,
                                 size_t nSize) {
    cudaError_t eResult;
    eResult =
        cudaMemcpy(pDst, pSrc, nSize * sizeof(Npp32fc), cudaMemcpyDeviceToHost);
    NPP_ASSERT(cudaSuccess == eResult);
  };
};

template <>
class SignalAllocator<Npp64s> {
 public:
  static Npp64s* Malloc1D(size_t nSize) {
    Npp64s* pResult = nppsMalloc_64s(static_cast<int>(nSize));
    NPP_ASSERT(pResult != 0);

    return pResult;
  };

  static void Free1D(Npp64s* pValues) { nppsFree(pValues); };

  static void Copy1D(Npp64s* pDst, const Npp64s* pSrc, size_t nSize) {
    cudaError_t eResult;
    eResult = cudaMemcpy(pDst, pSrc, nSize * sizeof(Npp64s),
                         cudaMemcpyDeviceToDevice);
    NPP_ASSERT(cudaSuccess == eResult);
  };

  static void HostToDeviceCopy1D(Npp64s* pDst, const Npp64s* pSrc,
                                 size_t nSize) {
    cudaError_t eResult;
    eResult =
        cudaMemcpy(pDst, pSrc, nSize * sizeof(Npp64s), cudaMemcpyHostToDevice);
    NPP_ASSERT(cudaSuccess == eResult);
  };

  static void DeviceToHostCopy1D(Npp64s* pDst, const Npp64s* pSrc,
                                 size_t nSize) {
    cudaError_t eResult;
    eResult =
        cudaMemcpy(pDst, pSrc, nSize * sizeof(Npp64s), cudaMemcpyDeviceToHost);
    NPP_ASSERT(cudaSuccess == eResult);
  };
};

template <>
class SignalAllocator<Npp64sc> {
 public:
  static Npp64sc* Malloc1D(size_t nSize) {
    Npp64sc* pResult = nppsMalloc_64sc(static_cast<int>(nSize));
    NPP_ASSERT(pResult != 0);

    return pResult;
  };

  static void Free1D(Npp64sc* pValues) { nppsFree(pValues); };

  static void Copy1D(Npp64sc* pDst, const Npp64sc* pSrc, size_t nSize) {
    cudaError_t eResult;
    eResult = cudaMemcpy(pDst, pSrc, nSize * sizeof(Npp64sc),
                         cudaMemcpyDeviceToDevice);
    NPP_ASSERT(cudaSuccess == eResult);
  };

  static void HostToDeviceCopy1D(Npp64sc* pDst, const Npp64sc* pSrc,
                                 size_t nSize) {
    cudaError_t eResult;
    eResult =
        cudaMemcpy(pDst, pSrc, nSize * sizeof(Npp64sc), cudaMemcpyHostToDevice);
    NPP_ASSERT(cudaSuccess == eResult);
  };

  static void DeviceToHostCopy1D(Npp64sc* pDst, const Npp64sc* pSrc,
                                 size_t nSize) {
    cudaError_t eResult;
    eResult =
        cudaMemcpy(pDst, pSrc, nSize * sizeof(Npp64sc), cudaMemcpyDeviceToHost);
    NPP_ASSERT(cudaSuccess == eResult);
  };
};

template <>
class SignalAllocator<Npp64f> {
 public:
  static Npp64f* Malloc1D(size_t nSize) {
    Npp64f* pResult = nppsMalloc_64f(static_cast<int>(nSize));
    NPP_ASSERT(pResult != 0);

    return pResult;
  };

  static void Free1D(Npp64f* pValues) { nppsFree(pValues); };

  static void Copy1D(Npp64f* pDst, const Npp64f* pSrc, size_t nSize) {
    cudaError_t eResult;
    eResult = cudaMemcpy(pDst, pSrc, nSize * sizeof(Npp64f),
                         cudaMemcpyDeviceToDevice);
    NPP_ASSERT(cudaSuccess == eResult);
  };

  static void HostToDeviceCopy1D(Npp64f* pDst, const Npp64f* pSrc,
                                 size_t nSize) {
    cudaError_t eResult;
    eResult =
        cudaMemcpy(pDst, pSrc, nSize * sizeof(Npp64f), cudaMemcpyHostToDevice);
    NPP_ASSERT(cudaSuccess == eResult);
  };

  static void DeviceToHostCopy1D(Npp64f* pDst, const Npp64f* pSrc,
                                 size_t nSize) {
    cudaError_t eResult;
    eResult =
        cudaMemcpy(pDst, pSrc, nSize * sizeof(Npp64f), cudaMemcpyDeviceToHost);
    NPP_ASSERT(cudaSuccess == eResult);
  };
};

template <>
class SignalAllocator<Npp64fc> {
 public:
  static Npp64fc* Malloc1D(size_t nSize) {
    Npp64fc* pResult = nppsMalloc_64fc(static_cast<int>(nSize));
    NPP_ASSERT(pResult != 0);

    return pResult;
  };

  static void Free1D(Npp64fc* pValues) { nppsFree(pValues); };

  static void Copy1D(Npp64fc* pDst, const Npp64fc* pSrc, size_t nSize) {
    cudaError_t eResult;
    eResult = cudaMemcpy(pDst, pSrc, nSize * sizeof(Npp64fc),
                         cudaMemcpyDeviceToDevice);
    NPP_ASSERT(cudaSuccess == eResult);
  };

  static void HostToDeviceCopy1D(Npp64fc* pDst, const Npp64fc* pSrc,
                                 size_t nSize) {
    cudaError_t eResult;
    eResult =
        cudaMemcpy(pDst, pSrc, nSize * sizeof(Npp64fc), cudaMemcpyHostToDevice);
    NPP_ASSERT(cudaSuccess == eResult);
  };

  static void DeviceToHostCopy1D(Npp64fc* pDst, const Npp64fc* pSrc,
                                 size_t nSize) {
    cudaError_t eResult;
    eResult =
        cudaMemcpy(pDst, pSrc, nSize * sizeof(Npp64fc), cudaMemcpyDeviceToHost);
    NPP_ASSERT(cudaSuccess == eResult);
  };
};
}  // namespace npp

#endif  // NV_UTIL_NPP_SIGNAL_ALLOCATORS_NPP_H
