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
 * CUDA particle system kernel code.
 */

#ifndef _PARTICLES_KERNEL_H_
#define _PARTICLES_KERNEL_H_

#include "helper_math.h"
#include "math_constants.h"
#include "particles_kernel.cuh"
#include <math.h>
#include <stdio.h>

#if USE_TEX
// textures for particle position and velocity
texture<float4, 1, cudaReadModeElementType> oldPosTex;
texture<float4, 1, cudaReadModeElementType> oldVelTex;

texture<uint, 1, cudaReadModeElementType> gridParticleHashTex;
texture<uint, 1, cudaReadModeElementType> cellStartTex;
texture<uint, 1, cudaReadModeElementType> cellEndTex;
#endif

// simulation parameters in constant memory
__constant__ SimParams params;

struct integrate_functor {
  float deltaTime;

  __host__ __device__ integrate_functor(float delta_time)
      : deltaTime(delta_time) {}

  template <typename Tuple>
  __device__ void operator()(Tuple t) {
    volatile float4 posData = thrust::get<0>(t);
    volatile float4 velData = thrust::get<1>(t);
    float3 pos = make_float3(posData.x, posData.y, posData.z);
    float3 vel = make_float3(velData.x, velData.y, velData.z);

    vel += params.gravity * deltaTime;
    vel *= params.globalDamping;

    // new position = old position + velocity * deltaTime
    pos += vel * deltaTime;

    // set this to zero to disable collisions with cube sides
#if 1

    if (pos.x > 1.0f - params.particleRadius) {
      pos.x = 1.0f - params.particleRadius;
      vel.x *= params.boundaryDamping;
    }

    if (pos.x < -1.0f + params.particleRadius) {
      pos.x = -1.0f + params.particleRadius;
      vel.x *= params.boundaryDamping;
    }

    if (pos.y > 1.0f - params.particleRadius) {
      pos.y = 1.0f - params.particleRadius;
      vel.y *= params.boundaryDamping;
    }

    if (pos.z > 1.0f - params.particleRadius) {
      pos.z = 1.0f - params.particleRadius;
      vel.z *= params.boundaryDamping;
    }

    if (pos.z < -1.0f + params.particleRadius) {
      pos.z = -1.0f + params.particleRadius;
      vel.z *= params.boundaryDamping;
    }

#endif

    if (pos.y < -1.0f + params.particleRadius) {
      pos.y = -1.0f + params.particleRadius;
      vel.y *= params.boundaryDamping;
    }

    // store new position and velocity
    thrust::get<0>(t) = make_float4(pos, posData.w);
    thrust::get<1>(t) = make_float4(vel, velData.w);
  }
};

// calculate position in uniform grid
__device__ int3 calcGridPos(float3 p) {
  int3 gridPos;
  gridPos.x = floor((p.x - params.worldOrigin.x) / params.cellSize.x);
  gridPos.y = floor((p.y - params.worldOrigin.y) / params.cellSize.y);
  gridPos.z = floor((p.z - params.worldOrigin.z) / params.cellSize.z);
  return gridPos;
}

// calculate address in grid from position (clamping to edges)
__device__ uint calcGridHash(int3 gridPos) {
  gridPos.x = gridPos.x &
              (params.gridSize.x - 1);  // wrap grid, assumes size is power of 2
  gridPos.y = gridPos.y & (params.gridSize.y - 1);
  gridPos.z = gridPos.z & (params.gridSize.z - 1);
  return __umul24(__umul24(gridPos.z, params.gridSize.y), params.gridSize.x) +
         __umul24(gridPos.y, params.gridSize.x) + gridPos.x;
}

// calculate grid hash value for each particle
__global__ void calcHashD(uint* gridParticleHash,   // output
                          uint* gridParticleIndex,  // output
                          float4* pos,              // input: positions
                          uint numParticles) {
  uint index = __umul24(blockIdx.x, blockDim.x) + threadIdx.x;

  if (index >= numParticles) return;

  volatile float4 p = pos[index];

  // get address in grid
  int3 gridPos = calcGridPos(make_float3(p.x, p.y, p.z));
  uint hash = calcGridHash(gridPos);

  // store grid hash and particle index
  gridParticleHash[index] = hash;
  gridParticleIndex[index] = index;
}

// rearrange particle data into sorted order, and find the start of each cell
// in the sorted hash array
__global__ void reorderDataAndFindCellStartD(
    uint* cellStart,          // output: cell start index
    uint* cellEnd,            // output: cell end index
    float4* sortedPos,        // output: sorted positions
    float4* sortedVel,        // output: sorted velocities
    uint* gridParticleHash,   // input: sorted grid hashes
    uint* gridParticleIndex,  // input: sorted particle indices
    float4* oldPos,           // input: sorted position array
    float4* oldVel,           // input: sorted velocity array
    uint numParticles) {
  extern __shared__ uint sharedHash[];  // blockSize + 1 elements
  uint index = __umul24(blockIdx.x, blockDim.x) + threadIdx.x;

  uint hash;

  // handle case when no. of particles not multiple of block size
  if (index < numParticles) {
    hash = gridParticleHash[index];

    // Load hash data into shared memory so that we can look
    // at neighboring particle's hash value without loading
    // two hash values per thread
    sharedHash[threadIdx.x + 1] = hash;

    if (index > 0 && threadIdx.x == 0) {
      // first thread in block must load neighbor particle hash
      sharedHash[0] = gridParticleHash[index - 1];
    }
  }

  __syncthreads();

  if (index < numParticles) {
    // If this particle has a different cell index to the previous
    // particle then it must be the first particle in the cell,
    // so store the index of this particle in the cell.
    // As it isn't the first particle, it must also be the cell end of
    // the previous particle's cell

    if (index == 0 || hash != sharedHash[threadIdx.x]) {
      cellStart[hash] = index;

      if (index > 0) cellEnd[sharedHash[threadIdx.x]] = index;
    }

    if (index == numParticles - 1) {
      cellEnd[hash] = index + 1;
    }

    // Now use the sorted index to reorder the pos and vel data
    uint sortedIndex = gridParticleIndex[index];
    float4 pos = FETCH(
        oldPos, sortedIndex);  // macro does either global read or texture fetch
    float4 vel = FETCH(oldVel, sortedIndex);  // see particles_kernel.cuh

    sortedPos[index] = pos;
    sortedVel[index] = vel;
  }
}

// collide two spheres using DEM method
__device__ float3 collideSpheres(float3 posA, float3 posB, float3 velA,
                                 float3 velB, float radiusA, float radiusB,
                                 float attraction) {
  // calculate relative position
  float3 relPos = posB - posA;

  float dist = length(relPos);
  float collideDist = radiusA + radiusB;

  float3 force = make_float3(0.0f);

  if (dist < collideDist) {
    float3 norm = relPos / dist;

    // relative velocity
    float3 relVel = velB - velA;

    // relative tangential velocity
    float3 tanVel = relVel - (dot(relVel, norm) * norm);

    // spring force
    force = -params.spring * (collideDist - dist) * norm;
    // dashpot (damping) force
    force += params.damping * relVel;
    // tangential shear force
    force += params.shear * tanVel;
    // attraction
    force += attraction * relPos;
  }

  return force;
}

// collide a particle against all other particles in a given cell
__device__ float3 collideCell(int3 gridPos, uint index, float3 pos, float3 vel,
                              float4* oldPos, float4* oldVel, uint* cellStart,
                              uint* cellEnd) {
  uint gridHash = calcGridHash(gridPos);

  // get start of bucket for this cell
  uint startIndex = FETCH(cellStart, gridHash);

  float3 force = make_float3(0.0f);

  if (startIndex != 0xffffffff)  // cell is not empty
  {
    // iterate over particles in this cell
    uint endIndex = FETCH(cellEnd, gridHash);

    for (uint j = startIndex; j < endIndex; j++) {
      if (j != index)  // check not colliding with self
      {
        float3 pos2 = make_float3(FETCH(oldPos, j));
        float3 vel2 = make_float3(FETCH(oldVel, j));

        // collide two spheres
        force += collideSpheres(pos, pos2, vel, vel2, params.particleRadius,
                                params.particleRadius, params.attraction);
      }
    }
  }

  return force;
}

__global__ void collideD(
    float4* newVel,           // output: new velocity
    float4* oldPos,           // input: sorted positions
    float4* oldVel,           // input: sorted velocities
    uint* gridParticleIndex,  // input: sorted particle indices
    uint* cellStart, uint* cellEnd, uint numParticles) {
  uint index = __mul24(blockIdx.x, blockDim.x) + threadIdx.x;

  if (index >= numParticles) return;

  // read particle data from sorted arrays
  float3 pos = make_float3(FETCH(oldPos, index));
  float3 vel = make_float3(FETCH(oldVel, index));

  // get address in grid
  int3 gridPos = calcGridPos(pos);

  // examine neighbouring cells
  float3 force = make_float3(0.0f);

  for (int z = -1; z <= 1; z++) {
    for (int y = -1; y <= 1; y++) {
      for (int x = -1; x <= 1; x++) {
        int3 neighbourPos = gridPos + make_int3(x, y, z);
        force += collideCell(neighbourPos, index, pos, vel, oldPos, oldVel,
                             cellStart, cellEnd);
      }
    }
  }

  // collide with cursor sphere
  force += collideSpheres(pos, params.colliderPos, vel,
                          make_float3(0.0f, 0.0f, 0.0f), params.particleRadius,
                          params.colliderRadius, 0.0f);

  // write new velocity back to original unsorted location
  uint originalIndex = gridParticleIndex[index];
  newVel[originalIndex] = make_float4(vel + force, 0.0f);
}

#endif
