#ifndef BLOCK_MATCHING_3D
#define BLOCK_MATCHING_3D

#include <fftw3.h>

#include "AlignedMemory.hpp"
#include "Block.hpp"
#include <map>

template <typename _Ty>
_Ty Max(const _Ty& a, const _Ty& b) {
  return b > a ? b : a;
}

typedef Block<FLType, FLType> block_type;
typedef block_type::KeyType KeyType;
typedef block_type::PosType PosType;
typedef block_type::PosPair PosPair;
typedef block_type::KeyCode KeyCode;
typedef block_type::PosCode PosCode;
typedef block_type::PosPairCode PosPairCode;
typedef BlockGroup<FLType, FLType> block_group;

struct FFTW3DPlanCache {
  int blockSize;
  int maxGroupSize;

  std::unordered_map<int, fftwf_plan> forwardPlans;
  std::unordered_map<int, fftwf_plan> inversePlans;
  std::unordered_map<int, float*> workspace;

  FFTW3DPlanCache(int blockSize_, int maxGroupSize_) : blockSize(blockSize_), maxGroupSize(maxGroupSize_) {
    int B = blockSize;

    for (int G = 1; G <= maxGroupSize; ++G) {
      int totalSize = G * B * B;
      float* buffer = (float*)fftwf_malloc(sizeof(float) * totalSize);

      fftwf_plan fPlan, iPlan;

      // Ensure FFTW plan creation is thread-safe
#pragma omp critical(fftw_plan_create)
      {
        // Forward plan (DCT-II)
        fPlan = fftwf_plan_r2r_3d(G, B, B, buffer, buffer, FFTW_REDFT10, FFTW_REDFT10, FFTW_REDFT10, FFTW_MEASURE);

        // Inverse plan (DCT-III)
        iPlan = fftwf_plan_r2r_3d(G, B, B, buffer, buffer, FFTW_REDFT01, FFTW_REDFT01, FFTW_REDFT01, FFTW_MEASURE);
      }

      if (!fPlan || !iPlan) {
        fprintf(stderr, "FFTW plan creation failed for G = %d\n", G);
        fftwf_free(buffer);
        throw std::runtime_error("FFTW plan creation failed");
      }

      forwardPlans[G] = fPlan;
      inversePlans[G] = iPlan;
      workspace[G] = buffer;
    }
  }

  fftwf_plan getForwardPlan(int groupSize) const { return forwardPlans.at(groupSize); }

  fftwf_plan getInversePlan(int groupSize) const { return inversePlans.at(groupSize); }

  float* getBuffer(int groupSize) const { return workspace.at(groupSize); }

  ~FFTW3DPlanCache() {
    for (auto& [G, plan] : forwardPlans) {
      fftwf_destroy_plan(plan);
    }
    for (auto& [G, plan] : inversePlans) {
      fftwf_destroy_plan(plan);
    }
    for (auto& [G, buf] : workspace) {
      fftwf_free(buf);
    }
  }
};


#endif  // BLOCK_MATCHING_3D