#ifndef ALIGNED_MEMORY
#define ALIGNED_MEMORY

#ifdef _WIN32
#include <malloc.h>
#else
#include <stdlib.h>
#endif

#define MEMORY_ALIGNMENT 64  // Adjust based on SIMD requirements

inline void AlignedMalloc(float*& Memory, size_t Count, size_t Alignment = MEMORY_ALIGNMENT) {
#ifdef _WIN32
  Memory = reinterpret_cast<float*>(_aligned_malloc(sizeof(float) * Count, Alignment));
#else
  if (posix_memalign(reinterpret_cast<void**>(&Memory), Alignment, sizeof(float) * Count)) {
    Memory = nullptr;
  }
#endif
}

inline void AlignedFree(float* Memory) {
#ifdef _WIN32
  _aligned_free(Memory);
#else
  free(Memory);
#endif
}

#endif  // ALIGNED_MEMORY
