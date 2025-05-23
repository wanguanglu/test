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

#ifndef PRICINGENGINE_H
#define PRICINGENGINE_H

#include "asianoption.h"

template <typename Real>
class PricingEngine {
 public:
  PricingEngine(unsigned int numSims, unsigned int device,
                unsigned int threadBlockSize, unsigned int seed);
  void operator()(AsianOption<Real>& option);

 private:
  unsigned int m_seed;
  unsigned int m_numSims;
  unsigned int m_device;
  unsigned int m_threadBlockSize;
};

#endif
