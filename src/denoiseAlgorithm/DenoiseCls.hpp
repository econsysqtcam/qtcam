#ifndef DENOISE_CLS_H
#define DENOISE_CLS_H

#include <math.h>
#include <omp.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>  
#include <time.h>

#include <algorithm>  
#include <limits>
#include <memory>
#include <unordered_map>
#include <vector>

#include "BM3D.hpp"

#define DEBUG 0
#define BLOCKNO 5000
#define TRFMCONST 1.4142135623730951  // sqrt(2)
#define INVTRFMCONST 0.7071067811865476 // 1/sqrt(2)

#define BPP 8
/* NonLocal */
#define SIGMA_H 10        // Filtering parameter for denoising strength
#define SEARCH_RADIUS 2   // Radius of the search window  2 means 5 x 5
#define WORKING_RADIUS 5  // Radius of the patch window 5 means 11 x 11
/* DWT */
#define PADDING 32     // DWT padding size 32
#define LEVEL 5        // Levels of DWT
#define OVERLAP 2      // Overlap size
#define THRESHOLD 1.0  // Filtering parameter for denoising strength
#define BLOCKSIZE 16   // Blocksize to find level scale log followed by threshold

#define Y_CHANNEL_DENOISE 1 /* 0 ==> nonLocalMeans   1 ==> BM3D */

#define BLOCK_SIZE 8  // block_size x block_size
#define BLOCK_STEP 6  // [1,block_size]
#define GROUP_SIZE 8  // [1,256]
#define BM_RANGE 7    // size of search window is (bm_range * 2 + 1) x (bm_range * 2 + 1).
#define BM_STEP 1     // [1, bm_range]
#define THRESHOLD_MSE 3000
#define LAMBDA 2.7
#define SYMETRIZE_PADDING 32  // padding size during symetrize

class DenoiseProcessor {
 private:
  int prevWidth = 0;
  int prevHeight = 0;
  int blockNumber = 0;
  // buffers to store the processed frames in the intermediate steps
  /* Y channel */
  float* srcY = nullptr;
  float* dstY = nullptr;
  float* dstYd = nullptr;
  float* srcYd = nullptr;
  int maxThread = 0;
  std::vector<std::vector<float>> subNoisy;
  std::vector<std::vector<float>> subDenoised;
  std::vector<unsigned> hTable;
  std::vector<unsigned> wTable;
  std::vector<float> srcVec;
  std::vector<float> dstVec;
  /* UV channel */
  std::vector<float> RYChannelImage;
  std::vector<float> RYFilteredImage;
  std::vector<float> UChannelImage;
  std::vector<float> VChannelImage;
  std::vector<float> UFilteredImage;
  std::vector<float> VFilteredImage;
  std::vector<float> paddedImage;
  std::vector<float> paddedImageOut;
  std::vector<float> transformTemp;
  std::vector<float> inverseTransformTemp;
  std::vector<double> coeffs;
  std::vector<double> values;

  void initializeOpenmp();
  static double getTimeInSeconds();
  int allocateMemoryBuffers(int width, int height);
  void freeMemoryBuffers();
  int BM3D(const float* inputBuffer, float* outputBuffer, int width, int height, float gain_val, float denoising_strength);
  void processCoreGray(float* dstY, float* srcY, size_t width, size_t height, float gain_val, float denoising_strength);
  static void symetrize(const std::vector<float>& img, std::vector<float>& imgSym, const unsigned width,
                        const unsigned height, const unsigned chnls, const unsigned symPadding);
  static void subDivideImage(std::vector<float>& img, std::vector<std::vector<float>>& subImg,
                             std::vector<unsigned>& wTable, std::vector<unsigned>& hTable, const unsigned width,
                             const unsigned height, const unsigned chnls, const unsigned symPadding, bool divide);
  static void kernel(float* dstYd, float* srcYd, float* refYd, size_t width, size_t height, float gain_val, float denoising_strength);
  static PosPairCode BlockMatching(float* refYd, int BlockPosBottomIndex, int BlockPosRightIndex, size_t width,
                                   size_t height);
  static void CollaborativeFilter(FLType* ResNum, FLType* ResDen, const FLType* src, const PosPairCode& code,
                                  FFTW3DPlanCache& planCache, float avgLuminance, const std::vector<float>& cur_lut,
                                  size_t width);
  static void CollaborativeFilterFinal(FLType* ResNum, FLType* ResDen, const FLType* src, const FLType* ref,
                                       const PosPairCode& code, FFTW3DPlanCache& planCache, float avgLuminance,
    const std::vector<float>& cur_lut, size_t width);
  static int nonLocalMeans(int* inputBuffer, int* outputBuffer, int width, int height);
  static float calculatePatchDistance(const int* inputBuffer, int col, int row, int currentCol, int currentRow,
                                      int width, int height);
  int WaveletDenoise(float* outputBuffer, const float* inputBuffer, int width, int height);
  int waveletTransform(double* block, int width, int height);
  int inverseWaveletTransform(double* block, int width, int height);
  static int compareDouble(const void* valueA, const void* valueB);
  static double findMedian(double* arr, int size);
  void applyLevelDependentThresholding(double* coeffs, int width, int height);
  double estimateNoiseVariance(double* coeffs, int width, int height, int stride);
  static void applySoftThresholding(double* coeffs, int width, int height, int stride, double threshold);
  int dwtDenoise(float* inputBuffer, int width, int height);

 public:
  DenoiseProcessor();
  ~DenoiseProcessor();
  int DenoiseYUV(float* outputBuffer, const float* inputBuffer, int width, int height, float gain_val, float denoising_strength);
};

#endif  // DENOISE_CLS_H
