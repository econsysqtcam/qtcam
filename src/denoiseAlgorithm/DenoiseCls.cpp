#include "DenoiseCls.hpp"
#include <fstream>
#include <vector>
#include <cstdint>
#include <algorithm>

#include "Denoise_LUT.h"

DenoiseProcessor::DenoiseProcessor() {
  // setting number of cores which should be used by OpenMP
  initializeOpenmp();
}

DenoiseProcessor::~DenoiseProcessor() {
  /* free allocated buffers */
  freeMemoryBuffers();
  //printf("\n***** END OF DENOISING *****\n");
}

float ComputePatchAverage(const float* refYd, int width, int height, int j, int i, int BlockSize) {
    float sum = 0.0f;
    for (int y = 0; y < BlockSize; ++y) {
        for (int x = 0; x < BlockSize; ++x) {
            int idx = (j + y) * width + (i + x);
            sum += refYd[idx];
        }
    }
    return sum / (BlockSize * BlockSize);
}


std::shared_ptr<FLType> generateThresholdTable(PCType groupSize, PCType blockSize, double sigma, double lambda) {
    // Correct amplification factor based on 3D DCT
    double forwardAMP = std::sqrt(8.0 * groupSize * blockSize * blockSize);
    double base = sigma * lambda * forwardAMP;

    std::vector<double> thr(4);
    thr[0] = base;
    thr[1] = base * std::sqrt(2.0);
    thr[2] = base * 2.0;
    thr[3] = base * std::sqrt(8.0);

    FLType* thrp_raw = nullptr;
    size_t total = groupSize * blockSize * blockSize;
    posix_memalign((void**)&thrp_raw, 32, total * sizeof(FLType));
    FLType* p = thrp_raw;

    for (PCType z = 0; z < groupSize; ++z) {
        for (PCType y = 0; y < blockSize; ++y) {
            for (PCType x = 0; x < blockSize; ++x, ++p) {
                int flag = 0;
                if (x == 0) ++flag;
                if (y == 0) ++flag;
                if (z == 0) ++flag;

                *p = static_cast<FLType>(thr[flag]);
            }
        }
    }

    return std::shared_ptr<FLType>(thrp_raw, [](FLType* ptr) {
        free(ptr);
    });
}



void DenoiseProcessor::initializeOpenmp() {
  const int totalCores = omp_get_num_procs();  // Get total cores
  this->maxThread = totalCores / 2;            // Use only 50% of the cores

  this->maxThread = std::max(this->maxThread, 1);  // Ensure at least 1 thread
  omp_set_num_threads(this->maxThread);            // Set OpenMP thread count

  //printf("Total Cores: %d, OpenMP Threads Used: %d\n", totalCores, this->maxThread);

  /* Resizing the vectors to store sub-Image details */
  this->subNoisy.resize(this->maxThread);
  this->subDenoised.resize(this->maxThread);
  this->hTable.resize(this->maxThread);
  this->wTable.resize(this->maxThread);
}

// Function to get current time in microseconds
double DenoiseProcessor::getTimeInSeconds() {
  struct timeval time = {};
  gettimeofday(&time, nullptr);
  return static_cast<double>(time.tv_sec) + (static_cast<double>(time.tv_usec) / 1e6);  // convert to seconds
}

// Precompute cur_lut at the start of BM3D
std::vector<float> precomputeNoiseLUT(float gain, float m, float c, float denoising_strength) {
    std::vector<float> cur_lut(256); // LUT for luminance 0 to 255

    // Find the two nearest gains
    int lowerGain = -1, upperGain = -1;
    for (const auto& entry : gainNoiseLUTs) {
        int currentGain = entry.first;
        if (currentGain <= gain) {
            if (lowerGain == -1 || currentGain > lowerGain) {
                lowerGain = currentGain;
            }
        }
        if (currentGain >= gain) {
            if (upperGain == -1 || currentGain < upperGain) {
                upperGain = currentGain;
            }
        }
    }

    // Handle edge cases
    if (lowerGain == -1) {
        // Use upperGain LUT
        const auto& lut = gainNoiseLUTs.at(upperGain);
        for (int i = 0; i < 256; ++i) {
            float sigma = (i < lut.size()) ? lut[i] : lut.back(); // Use last value if index exceeds LUT
            cur_lut[i] =m * (sigma * denoising_strength) + c; // Apply y = mx + c
        }
        return cur_lut;
    }
    if (upperGain == -1) {
        // Use lowerGain LUT
        const auto& lut = gainNoiseLUTs.at(lowerGain);
        for (int i = 0; i < 256; ++i) {
            float sigma = (i < lut.size()) ? lut[i] : lut.back();
            cur_lut[i] =m * (sigma * denoising_strength) + c;
        }
        return cur_lut;
    }

    // Exact gain match
    if (lowerGain == upperGain) {
        const auto& lut = gainNoiseLUTs.at(lowerGain);
        for (int i = 0; i < 256; ++i) {
            float sigma = (i < lut.size()) ? lut[i] : lut.back();
            cur_lut[i] =m * (sigma * denoising_strength) + c;
        }
        return cur_lut;
    }

    // Interpolate between lowerGain and upperGain
    const auto& lowerLUT = gainNoiseLUTs.at(lowerGain);
    const auto& upperLUT = gainNoiseLUTs.at(upperGain);
    float t = (gain - lowerGain) / static_cast<float>(upperGain - lowerGain);


    for (int i = 0; i < 256; ++i) {
        float lowerSigma = (i < lowerLUT.size()) ? lowerLUT[i] : lowerLUT.back();
        float upperSigma = (i < upperLUT.size()) ? upperLUT[i] : upperLUT.back();
        float sigma = lowerSigma + t * (upperSigma - lowerSigma); // Interpolate
        cur_lut[i] =m * (sigma * denoising_strength) + c; // Apply y = mx + c
    }

    return cur_lut;
}


int DenoiseProcessor::DenoiseYUV(float* outputBuffer, const float* inputBuffer, int width, int height, float gain_val, float denoising_strength) {
  const int rpMax = ((1 << BPP) - 1);
  int idx = 0;
  double start = 0;
  double end = 0;
  int temp = 0;
  int data = 0;
  int status = 0;

  // Check if we need to allocate memory
  if (width != this->prevWidth || height != this->prevHeight) {
    //printf("New Resolution\n");
    // Update stored dimensions
    this->prevWidth = width;
    this->prevHeight = height;

    status = allocateMemoryBuffers(width, height);
    if (status != 1) {
      //printf("Memory allocation failed in DenoiseYUV\n");
      return -1;
    }
    //printf("Memory allocation successfull!\n");
  } else {
    //printf("Resolution Remains the same\n");
  }

  // Seaprating channels from YUV
  #pragma omp parallel for simd
  for (int k = 0; k < width * height; k++) {
      int idx = k * 3;
      this->RYChannelImage[k] = inputBuffer[idx + 0];
      this->UChannelImage[k]  = inputBuffer[idx + 1];
      this->VChannelImage[k]  = inputBuffer[idx + 2];
  }

#if Y_CHANNEL_DENOISE == 0
  // Perform non-local means denoising
  start = getTimeInSeconds();
  nonLocalMeans(this->RYChannelImage.data(), this->RYFilteredImage.data(), width, height);
  end = getTimeInSeconds();
  //printf("Time taken by nonLocalMeans: %f seconds\n", end - start);
#else
  start = getTimeInSeconds();
  BM3D(this->RYChannelImage.data(), this->RYFilteredImage.data(), width, height, gain_val, denoising_strength);
  end = getTimeInSeconds();
  //printf("Time taken by BM3D: %f seconds\n", end - start);

#endif

  start = getTimeInSeconds();
  WaveletDenoise(this->UFilteredImage.data(), this->UChannelImage.data(), width, height);
  end = getTimeInSeconds();
  //printf("Time taken by WaveletDenoise for U channel: %f seconds\n", end - start);

  start = getTimeInSeconds();
  WaveletDenoise(this->VFilteredImage.data(), this->VChannelImage.data(), width, height);
  end = getTimeInSeconds();
  //printf("Time taken by WaveletDenoise for V channel: %f seconds\n", end - start);


  idx = 0;
  #pragma omp parallel for simd
  for (int i = 0; i < width * height; i++) {
      int idx = i * 3;
      outputBuffer[idx + 0] = std::clamp(this->RYFilteredImage[i], 0.0f, static_cast<float>(rpMax));
      outputBuffer[idx + 1] = std::clamp(this->UFilteredImage[i],  0.0f, static_cast<float>(rpMax));
      outputBuffer[idx + 2] = std::clamp(this->VFilteredImage[i],  0.0f, static_cast<float>(rpMax));
  }

  return 1;
}

int DenoiseProcessor::allocateMemoryBuffers(int width, int height) {
  /* Y channel */
  const size_t pixelCount = static_cast<size_t>(width) * height;

  /* UV Channel */
  const int size = width * height;
  const int padding = PADDING;
  // Dimensions for the padded image
  const int paddedWidth = width + (2 * padding);
  const int paddedHeight = height + (2 * padding);
  const int paddedSize = paddedWidth * paddedHeight;
  int noiseVarianceBlocksize = 0;

  /* free previously allocated buffers */
  freeMemoryBuffers();

  /* Y channel */
  AlignedMalloc(this->srcY, pixelCount);
  AlignedMalloc(this->dstY, pixelCount);
  AlignedMalloc(this->dstYd, pixelCount);
  AlignedMalloc(this->srcYd, pixelCount);

  if (this->srcY == nullptr || this->dstY == nullptr || this->dstYd == nullptr || this->srcYd == nullptr) {
    //printf("Memory allocation failed!\n");
    return -1;
  }

  try {
    this->srcVec.resize(pixelCount);
    this->dstVec.resize(pixelCount);

    // Memory allocation successful if no exception was thrown
  } catch (const std::bad_alloc& e) {
    //printf("Memory allocation failed in allocateMemoryBuffers for Y-Channel: %s\n", e.what());
    return -1;
  }

  /* UV Channel */
  int reducedBlocksize = BLOCKSIZE;
  int halvingCount = 1;

  // Halve BLOCKSIZE LEVEL times
  while (reducedBlocksize > 1 && halvingCount <= LEVEL) {
    reducedBlocksize /= 2;
    halvingCount++;
  }

  // If BLOCKSIZE reached 1 after LEVEL halvings, calculate buffer size
  if (reducedBlocksize == 1 && halvingCount == LEVEL) {
    int finalWidth = paddedWidth;
    int finalHeight = paddedHeight;

    // Halve the width and height LEVEL times
    for (int i = 0; i < LEVEL; i++) {
      finalWidth = std::max(1, finalWidth / 2);
      finalHeight = std::max(1, finalHeight / 2);
    }

    noiseVarianceBlocksize = finalWidth * finalHeight;
  }

  try {
    this->RYChannelImage.resize(size);
    this->RYFilteredImage.resize(size);
    this->UChannelImage.resize(size);
    this->VChannelImage.resize(size);
    this->UFilteredImage.resize(size);
    this->VFilteredImage.resize(size);
    this->paddedImage.resize(paddedSize);
    this->paddedImageOut.resize(paddedSize);
    this->coeffs.resize(paddedSize);
    this->transformTemp.resize(paddedSize);
    this->inverseTransformTemp.resize(paddedSize);
    this->values.resize(noiseVarianceBlocksize);

    // Memory allocation successful if no exception was thrown
  } catch (const std::bad_alloc& e) {
    //printf("Memory allocation failed in allocateMemoryBuffers for UV-Channel: %s\n", e.what());
    return -1;
  }

  return 1;
}

void DenoiseProcessor::freeMemoryBuffers() {
  // Free allocated memory
  AlignedFree(this->srcY);
  AlignedFree(this->dstY);
  AlignedFree(this->dstYd);
  AlignedFree(this->srcYd);
}

int DenoiseProcessor::BM3D(const float* inputBuffer, float* outputBuffer, int width, int height, float gain_val, float denoising_strength) {
  const int size = width * height;
  const size_t pixelCount = static_cast<size_t>(width) * height;

#pragma omp parallel for simd
  for (size_t i = 0; i < pixelCount; ++i) {
    this->srcY[i] = static_cast<float>(inputBuffer[i]);  // Convert to float
  }

  processCoreGray(this->dstY, this->srcY, width, height, gain_val, denoising_strength);

#pragma omp parallel for simd
  for (size_t i = 0; i < pixelCount; i++) {
    outputBuffer[i] = static_cast<float>(this->dstY[i]);  // Convert back to int
  }

  return 1;
}

void DenoiseProcessor::processCoreGray(float* dstY, float* srcY, size_t width, size_t height, float gain_val, float denoising_strength) {
  const size_t pixelCount = width * height;

  memcpy(this->srcYd, srcY, pixelCount * sizeof(float));
  std::copy(this->srcYd, this->srcYd + pixelCount, this->srcVec.begin());
  std::copy(this->dstYd, this->dstYd + pixelCount, this->dstVec.begin());

  /* Split the source image across the allocated thread */
  subDivideImage(this->srcVec, this->subNoisy, this->wTable, this->hTable, width, height, 1, SYMETRIZE_PADDING, true);

#pragma omp parallel shared(subNoisy, subDenoised, wTable, hTable)
  {
#pragma omp for schedule(dynamic) nowait
    for (unsigned threadIdx = 0; threadIdx < maxThread; threadIdx++) {
      const size_t size = static_cast<size_t>(this->wTable[threadIdx]) *
                          static_cast<size_t>(this->hTable[threadIdx]);  // Replace with actual size
      this->subDenoised[threadIdx].resize(size);
      /* Copy Reference data (for simplicity, as of now, we dont have any reference data)*/
      std::vector<float> refYdBuffer(size);
      std::copy(this->subNoisy[threadIdx].begin(), this->subNoisy[threadIdx].end(), refYdBuffer.begin());
      kernel(this->subDenoised[threadIdx].data(), this->subNoisy[threadIdx].data(), refYdBuffer.data(),
             wTable[threadIdx], this->hTable[threadIdx], gain_val, denoising_strength);
    }
  }

  //! Reconstruction of the image
  subDivideImage(this->dstVec, this->subDenoised, this->wTable, this->hTable, width, height, 1, 2 * 16, false);

  memcpy(dstY, this->dstVec.data(), pixelCount * sizeof(float));
}

/**
 * @brief Add boundaries by symetry
 *
 * @param img : image to symetrize
 * @param imgSym : will contain img with symetrized boundaries
 * @param width, height, chnls : size of img
 * @param symPadding : size of the padding boundary
 *
 * @return none.
 **/
void DenoiseProcessor::symetrize(const std::vector<float>& img, std::vector<float>& imgSym, const unsigned width,
                                 const unsigned height, const unsigned chnls, const unsigned symPadding) {
  //! Declaration
  const unsigned paddedWidth = width + (2 * symPadding);
  const unsigned paddedHeight = height + (2 * symPadding);

  if (imgSym.size() !=
      static_cast<size_t>(paddedWidth) * static_cast<size_t>(paddedHeight) * static_cast<size_t>(chnls)) {
    imgSym.resize(static_cast<size_t>(paddedWidth) * static_cast<size_t>(paddedHeight) * static_cast<size_t>(chnls));
  }

  for (unsigned chnlIdx = 0; chnlIdx < chnls; chnlIdx++) {
    unsigned channelOffsetSrc = chnlIdx * width * height;
    unsigned channelOffsetDst = (chnlIdx * paddedWidth * paddedHeight) + (symPadding * paddedWidth) + symPadding;

    //! Center of the image
    for (unsigned i = 0; i < height; i++) {
      for (unsigned j = 0; j < width; j++, channelOffsetSrc++) {
        imgSym[channelOffsetDst + (i * paddedWidth) + j] = img[channelOffsetSrc];
      }
    }

    //! Top and bottom
    channelOffsetDst = chnlIdx * paddedWidth * paddedHeight;
    for (unsigned j = 0; j < paddedWidth; j++, channelOffsetDst++) {
      for (unsigned i = 0; i < symPadding; i++) {
        imgSym[channelOffsetDst + (i * paddedWidth)] =
            imgSym[channelOffsetDst + ((2 * symPadding - i - 1) * paddedWidth)];
        imgSym[channelOffsetDst + ((paddedHeight - i - 1) * paddedWidth)] =
            imgSym[channelOffsetDst + ((paddedHeight - 2 * symPadding + i) * paddedWidth)];
      }
    }

    //! Right and left
    channelOffsetDst = chnlIdx * paddedWidth * paddedHeight;
    for (unsigned i = 0; i < paddedHeight; i++) {
      const unsigned rowOffsetDst = channelOffsetDst + (i * paddedWidth);
      for (unsigned j = 0; j < symPadding; j++) {
        imgSym[rowOffsetDst + j] = imgSym[rowOffsetDst + (2 * symPadding) - j - 1];
        imgSym[rowOffsetDst + paddedWidth - j - 1] = imgSym[rowOffsetDst + paddedWidth - (2 * symPadding) + j];
      }
    }
  }
}

/**
 * @brief Subdivide an image into small sub-images
 *
 * @param img : image to subdivide
 * @param subImg: will contain all sub_images
 * @param wTable, hTable : size of sub-images contained in subImg
 * @param width, height, chnls: size of img
 * @param symPadding : size of the padding boundary
 * @param divide: if true, sub-divides img into subImg, else rebuild
 *        img from sub_images
 *
 * @return none.
 **/
void DenoiseProcessor::subDivideImage(std::vector<float>& img, std::vector<std::vector<float>>& subImg,
                                      std::vector<unsigned>& wTable, std::vector<unsigned>& hTable,
                                      const unsigned width, const unsigned height, const unsigned chnls,
                                      const unsigned symPadding, bool divide) {
  //! Add by symetry boundaries to the img
  const unsigned boundaryHeight = height + (2 * symPadding);
  const unsigned boundaryWidth = width + (2 * symPadding);
  std::vector<float> imgSym;
  if (divide) {
    symetrize(img, imgSym, width, height, chnls, symPadding);
  }

  //! Obtain nb of sub_images in row and column
  unsigned wSmall = width;
  unsigned hSmall = height;
  unsigned maxThread = subImg.size();
  unsigned widthSplits = 1;
  unsigned heightSplits = 1;
  while (maxThread > 1) {
    if (wSmall > hSmall) {
      wSmall = static_cast<unsigned>(floor(static_cast<float>(wSmall) * 0.5f));
      widthSplits *= 2;
    } else {
      hSmall = static_cast<unsigned>(floor(static_cast<float>(hSmall) * 0.5f));
      heightSplits *= 2;
    }
    maxThread /= 2;
  }

  //! As the image may don't have power of 2 dimensions, it may exist a boundary
  const unsigned hBound = (heightSplits > 1 ? height - ((heightSplits - 1) * hSmall) : hSmall);
  const unsigned wBound = (widthSplits > 1 ? width - ((widthSplits - 1) * wSmall) : wSmall);

  if (divide)  //! Subdivides the image in small parts
  {
    for (unsigned i = 0; i < heightSplits; i++) {
      for (unsigned j = 0; j < widthSplits; j++) {
        const unsigned subImageIdx = (i * widthSplits) + j;
        const unsigned paddedHeight = (i == heightSplits - 1 ? hBound : hSmall) + (2 * symPadding);
        const unsigned paddedWidth = (j == widthSplits - 1 ? wBound : wSmall) + (2 * symPadding);
        hTable[subImageIdx] = paddedHeight;
        wTable[subImageIdx] = paddedWidth;
        subImg[subImageIdx].resize(static_cast<size_t>(paddedWidth) * static_cast<size_t>(paddedHeight) *
                                   static_cast<size_t>(chnls));

        for (unsigned chnlIdx = 0; chnlIdx < chnls; chnlIdx++) {
          const unsigned symImageStartIdx =
              (chnlIdx * boundaryWidth * boundaryHeight) + (i * hSmall * boundaryWidth) + (j * wSmall);
          for (unsigned rowIdx = 0; rowIdx < paddedHeight; rowIdx++) {
            unsigned paddedPixelIdx = (chnlIdx * paddedWidth * paddedHeight) + (rowIdx * paddedWidth);
            for (unsigned colIdx = 0; colIdx < paddedWidth; colIdx++, paddedPixelIdx++) {
              subImg[subImageIdx][paddedPixelIdx] = imgSym[symImageStartIdx + (rowIdx * boundaryWidth) + colIdx];
            }
          }
        }
      }
    }
  } else  //! Reconstruction of the image
  {
    for (unsigned i = 0; i < heightSplits; i++) {
      for (unsigned j = 0; j < widthSplits; j++) {
        const unsigned subImageIdx = (i * widthSplits) + j;
        const unsigned paddedHeight = (i == heightSplits - 1 ? hBound : hSmall) + (2 * symPadding);
        const unsigned paddedWidth = (j == widthSplits - 1 ? wBound : wSmall) + (2 * symPadding);
        for (unsigned chnlIdx = 0; chnlIdx < chnls; chnlIdx++) {
          const unsigned paddedImageStartIdx =
              (chnlIdx * paddedWidth * paddedHeight) + (symPadding * paddedWidth) + symPadding;
          const unsigned symImageStartIdx = (chnlIdx * width * height) + (i * hSmall * width) + (j * wSmall);
          for (unsigned rowIdx = 0; rowIdx < paddedHeight - 2 * symPadding; rowIdx++) {
            unsigned paddedPixelIdx = paddedImageStartIdx + (rowIdx * paddedWidth);
            for (unsigned colIdx = 0; colIdx < paddedWidth - 2 * symPadding; colIdx++, paddedPixelIdx++) {
              img[symImageStartIdx + (rowIdx * width) + colIdx] = subImg[subImageIdx][paddedPixelIdx];
            }
          }
        }
      }
    }
  }
}

void DenoiseProcessor::kernel(float* dstYd, float* srcYd, float* refYd, size_t width, size_t height, float gain_val, float denoising_strength) {
  float* ResNum = dstYd;
  float* ResDen = nullptr;
  const size_t pixelCount = width * height;


  float cur_gain = gain_val;
  float slope = 0.0;
  float intercept = 0.0;

  if (cur_gain >= 0.0 && cur_gain <= 4.0) {
    slope = 5.723; intercept = 2.339;       // Gain 0
  } 
  else if (cur_gain >= 5.0 && cur_gain <= 14.0) {
      slope = 6.885; intercept = 1.467;       // Gain 10
  } 
  else if (cur_gain >= 15.0 && cur_gain <= 24.0) {
      slope = 7.883; intercept = 0.914;       // Gain 20
  } 
  else if (cur_gain >= 25.0 && cur_gain <= 34.0) {
      slope = 10.515; intercept = -3.781;     // Gain 30
  } 
  else if (cur_gain >= 35.0 && cur_gain <= 44.0) {
      slope = 5.527; intercept = 3.934;       // Gain 40
  } 
  else if (cur_gain >= 45.0 && cur_gain <= 54.0) {
      slope = 6.009; intercept = 5.289;       // Gain 50
  } 
  else if (cur_gain >= 55.0 && cur_gain <= 64.0) {
      slope = 6.198; intercept = 7.743;       // Gain 60
  } 
  else if (cur_gain >= 65.0 && cur_gain <= 74.0) {
      slope = 4.925; intercept = 14.311;      // Gain 70
  } 
  else if (cur_gain >= 75.0 && cur_gain <= 84.0) {
      slope = 8.600; intercept = 7.252;       // Gain 80
  } 
  else if (cur_gain >= 85.0 && cur_gain <= 94.0) {
      slope = 6.646; intercept = 15.933;      // Gain 90
  } 
  else if (cur_gain >= 95.0 && cur_gain <= 104.0) {
      slope = 8.207; intercept = 1.755;       // Gain 100
  } 
  else if (cur_gain >= 105.0 && cur_gain <= 114.0) {
      slope = 7.707; intercept = 6.283;       // Gain 110
  } 
  else if (cur_gain >= 115.0 && cur_gain <= 124.0) {
      slope = 7.114; intercept = 10.450;      // Gain 120
  } 
  else if (cur_gain >= 125.0 && cur_gain <= 130.0) {
      slope = 6.757; intercept = 10.252;      // Gain 130
  }
  else {
    //printf("Incorrect Gain value!\n");
  }

  //printf("cur_gain: %f, slope: %f, intercept: %f, sigma_factor: %f\n", cur_gain, slope, intercept, denoising_strength);

  AlignedMalloc(ResDen, pixelCount);
  if (ResDen == nullptr) {
    //printf("Memory allocation failed for ResDen!\n");
    return;
  }

  memset(ResDen, 0, pixelCount * sizeof(float));
  memset(ResNum, 0, pixelCount * sizeof(float));

  const int BlockPosBottom = static_cast<int>(height) - BLOCK_SIZE;
  const int BlockPosRight = static_cast<int>(width) - BLOCK_SIZE;
  FFTW3DPlanCache planCache(BLOCK_SIZE, GROUP_SIZE);
  std::vector<float> cur_lut = precomputeNoiseLUT(cur_gain, slope, intercept, denoising_strength);

  for (int j = 0; j <= BlockPosBottom; j += BLOCK_STEP) {
    for (int i = 0; i <= BlockPosRight; i += BLOCK_STEP) {
      const PosPairCode matchCode = BlockMatching(refYd, j, i, width, height);
      const int matchCodeSize = static_cast<PCType>(matchCode.size());
      float avgLuminance = ComputePatchAverage(refYd, width, height, j, i, BLOCK_SIZE);
      CollaborativeFilter(ResNum, ResDen, srcYd, matchCode, planCache, avgLuminance, cur_lut, width);
    }
  }

  for (int i = 0; i < pixelCount; ++i) {
    refYd[i] = ResNum[i] / ResDen[i];
  }

  memset(ResDen, 0, pixelCount * sizeof(float));
  memset(ResNum, 0, pixelCount * sizeof(float));

  // // 1st step of BM3D is done, we will get the output in refYd as float

  // // Now, 2nd step
  // // Second step of BM happens using the output of the first step --> refYd

  // // SrcYd ==> Noisy Data
  // // RefYd ==> Denoised Data of 1st step
  // // DstYd ==> Final Denoised Data

  for (int j = 0; j <= BlockPosBottom; j += BLOCK_STEP) {
    for (int i = 0; i <= BlockPosRight; i += BLOCK_STEP) {
      const PosPairCode matchCode = BlockMatching(refYd, j, i, width, height);
      float avgLuminance = ComputePatchAverage(refYd, width, height, j, i, BLOCK_SIZE);
      CollaborativeFilterFinal(ResNum, ResDen, srcYd, refYd, matchCode, planCache, avgLuminance, cur_lut, width);
    }
  }

  for (int i = 0; i < pixelCount; ++i) {
    dstYd[i] = ResNum[i] / ResDen[i];
  }

  AlignedFree(ResDen);
}

PosPairCode DenoiseProcessor::BlockMatching(float* refYd, int BlockPosBottomIndex, int BlockPosRightIndex, size_t width,
                                            size_t height) {

  if (GROUP_SIZE == 1 || THRESHOLD_MSE <= 0) {
    return PosPairCode(1, PosPair(static_cast<KeyType>(0), PosType(BlockPosBottomIndex, BlockPosRightIndex)));
  }

  const block_type refBlock(refYd, static_cast<PCType>(width), BLOCK_SIZE, BLOCK_SIZE,
                            PosType(BlockPosBottomIndex, BlockPosRightIndex));

  return refBlock.BlockMatchingMulti(refYd, static_cast<PCType>(height), static_cast<PCType>(width),
                                     static_cast<PCType>(width), static_cast<FLType>(255.0), BM_RANGE, BM_STEP,
                                     THRESHOLD_MSE, 1, GROUP_SIZE, true);
}



void DenoiseProcessor::CollaborativeFilter(FLType* ResNum, FLType* ResDen, const FLType* src, const PosPairCode& code,
                                           FFTW3DPlanCache& planCache, float avgLuminance, const std::vector<float>& cur_lut,
                                           size_t width) {
  int Group_Size = static_cast<PCType>(code.size());

  if (GROUP_SIZE > 0 && Group_Size > GROUP_SIZE) {
    Group_Size = GROUP_SIZE;
  }

  block_group srcGroup(src, static_cast<PCType>(width), code, Group_Size, BLOCK_SIZE, BLOCK_SIZE);

  const int grpSize = Group_Size;
  const int blockSize = BLOCK_SIZE;

  float* data = planCache.getBuffer(grpSize);

  for (int grpIndex = 0; grpIndex < grpSize; ++grpIndex) {
    for (int blockIndexY = 0; blockIndexY < blockSize; ++blockIndexY) {
      for (int blockIndexX = 0; blockIndexX < blockSize; ++blockIndexX) {
        data[(grpIndex * blockSize * blockSize) + (blockIndexY * blockSize) + blockIndexX] =
            srcGroup(grpIndex, blockIndexY, blockIndexX);
      }
    }
  }

  fftwf_execute(planCache.getForwardPlan(grpSize));

  for (int grpIndex = 0; grpIndex < grpSize; ++grpIndex) {
    for (int blockIndexY = 0; blockIndexY < blockSize; ++blockIndexY) {
      for (int blockIndexX = 0; blockIndexX < blockSize; ++blockIndexX) {
        srcGroup(grpIndex, blockIndexY, blockIndexX) =
            data[(grpIndex * blockSize * blockSize) + (blockIndexY * blockSize) + blockIndexX];
      }
    }
  }

  int retainedCoefs = 0;

  int index = static_cast<int>(avgLuminance + 0.5f);

  float sigma = cur_lut[index];
  double sigma_d = static_cast<double>(sigma);

  auto thrp_auto = generateThresholdTable(Group_Size, BLOCK_SIZE, sigma_d, LAMBDA);
  float* thrp = thrp_auto.get();
  auto srcp = srcGroup.data();
  const auto upper = srcp + srcGroup.size();

#if defined(__SSE2__)
  static const __m128 abs_mask = _mm_castsi128_ps(_mm_set1_epi32(~0x80000000));
  static const ptrdiff_t simd_step = 4;
  const auto simd_residue = static_cast<ptrdiff_t>(srcGroup.size() % simd_step);
  const ptrdiff_t simd_width = static_cast<ptrdiff_t>(srcGroup.size()) - simd_residue;

  __m128i cmp_sum = _mm_setzero_si128();

  for (auto* const upper1 = srcp + simd_width; srcp < upper1; srcp += simd_step, thrp += simd_step) {
    const __m128 srcVec = _mm_load_ps(srcp);
    const __m128 threshVec = _mm_load_ps(thrp);

    const __m128 s1abs = _mm_and_ps(srcVec, abs_mask);
    const __m128 cmp = _mm_cmpgt_ps(s1abs, threshVec);

    const __m128 resultVec = _mm_and_ps(cmp, srcVec);
    _mm_store_ps(srcp, resultVec);
    cmp_sum = _mm_sub_epi32(cmp_sum, _mm_castps_si128(cmp));
  }

  alignas(16) std::array<int32_t, 4> cmp_sum_i32 = {};
  _mm_store_si128(reinterpret_cast<__m128i*>(cmp_sum_i32.data()), cmp_sum);
  retainedCoefs += cmp_sum_i32[0] + cmp_sum_i32[1] + cmp_sum_i32[2] + cmp_sum_i32[3];
#endif

  for (; srcp < upper; ++srcp, ++thrp) {
    if (*srcp > *thrp || *srcp < -*thrp) {
      ++retainedCoefs;
    } else {
      *srcp = 0;
    }
  }

  // ///////////////////////////////////////////////////////////////////////////////////////////////////
  // INVERSE DCT
  // Create inverse 1D DCT plans

  // float* data = planCache.getBuffer(grpSize);

  for (int grpIndex = 0; grpIndex < grpSize; ++grpIndex) {
    for (int blockIndexY = 0; blockIndexY < blockSize; ++blockIndexY) {
      for (int blockIndexX = 0; blockIndexX < blockSize; ++blockIndexX) {
        data[(grpIndex * blockSize * blockSize) + (blockIndexY * blockSize) + blockIndexX] =
            srcGroup(grpIndex, blockIndexY, blockIndexX);
      }
    }
  }

  fftwf_execute(planCache.getInversePlan(grpSize));
  // float scale = 1.0f / (8.0f * N0 * N1 * N2);

  for (int grpIndex = 0; grpIndex < grpSize; ++grpIndex) {
    for (int blockIndexY = 0; blockIndexY < blockSize; ++blockIndexY) {
      for (int blockIndexX = 0; blockIndexX < blockSize; ++blockIndexX) {
        srcGroup(grpIndex, blockIndexY, blockIndexX) =
            data[(grpIndex * blockSize * blockSize) + (blockIndexY * blockSize) + blockIndexX];
      }
    }
  }

  /////////////////////////////////////////////////////////////////////////////////////////////////////
  const FLType denWeight = retainedCoefs < 1 ? 1 : static_cast<FLType>(1) / static_cast<FLType>(retainedCoefs);
  const auto finalAMP = static_cast<float>(2 * Group_Size * 2 * BLOCK_SIZE * 2 * BLOCK_SIZE);
  const auto numWeight = static_cast<FLType>(denWeight / finalAMP);

  // Store the weighted filtered group to the numerator part of the basic estimation
  // Store the weight to the denominator part of the basic estimation
  srcGroup.AddTo(ResNum, static_cast<PCType>(width), numWeight);
  srcGroup.CountTo(ResDen, static_cast<PCType>(width), denWeight);
}

void DenoiseProcessor::CollaborativeFilterFinal(FLType* ResNum, FLType* ResDen, const FLType* src, const FLType* ref,
                                                const PosPairCode& code, FFTW3DPlanCache& planCache, float avgLuminance,
    const std::vector<float>& cur_lut, size_t width) {
  int Group_Size_Final = static_cast<PCType>(code.size());

  if (GROUP_SIZE > 0 && Group_Size_Final > GROUP_SIZE) {
    Group_Size_Final = GROUP_SIZE;
  }

  // Construct source group and reference group guided by matched pos code
  block_group srcGroup(src, static_cast<PCType>(width), code, Group_Size_Final, BLOCK_SIZE, BLOCK_SIZE);
  block_group refGroup(ref, static_cast<PCType>(width), code, Group_Size_Final, BLOCK_SIZE, BLOCK_SIZE);

  // Initialize L2-norm of Wiener coefficients
  FLType L2Wiener = 0;

  const int grpSize = Group_Size_Final;
  const int blockSize = BLOCK_SIZE;

  int index = static_cast<int>(avgLuminance + 0.5f);

  index = std::max(0, std::min(index, 255)); // Clamp to 0-255
  float sigma = cur_lut[index];
  
  // Forward 3D DCT for srcGroup
  float* data = planCache.getBuffer(grpSize);
  for (int grpIndex = 0; grpIndex < grpSize; ++grpIndex) {
    for (int blockIndexY = 0; blockIndexY < blockSize; ++blockIndexY) {
      for (int blockIndexX = 0; blockIndexX < blockSize; ++blockIndexX) {
        data[(grpIndex * blockSize * blockSize) + (blockIndexY * blockSize) + blockIndexX] =
            srcGroup(grpIndex, blockIndexY, blockIndexX);
      }
    }
  }
  fftwf_execute(planCache.getForwardPlan(grpSize));
  for (int grpIndex = 0; grpIndex < grpSize; ++grpIndex) {
    for (int blockIndexY = 0; blockIndexY < blockSize; ++blockIndexY) {
      for (int blockIndexX = 0; blockIndexX < blockSize; ++blockIndexX) {
        srcGroup(grpIndex, blockIndexY, blockIndexX) =
            data[(grpIndex * blockSize * blockSize) + (blockIndexY * blockSize) + blockIndexX];
      }
    }
  }

  // Forward 3D DCT for refGroup
  for (int grpIndex = 0; grpIndex < grpSize; ++grpIndex) {
    for (int blockIndexY = 0; blockIndexY < blockSize; ++blockIndexY) {
      for (int blockIndexX = 0; blockIndexX < blockSize; ++blockIndexX) {
        data[(grpIndex * blockSize * blockSize) + (blockIndexY * blockSize) + blockIndexX] =
            refGroup(grpIndex, blockIndexY, blockIndexX);
      }
    }
  }
  fftwf_execute(planCache.getForwardPlan(grpSize));
  for (int grpIndex = 0; grpIndex < grpSize; ++grpIndex) {
    for (int blockIndexY = 0; blockIndexY < blockSize; ++blockIndexY) {
      for (int blockIndexX = 0; blockIndexX < blockSize; ++blockIndexX) {
        refGroup(grpIndex, blockIndexY, blockIndexX) =
            data[(grpIndex * blockSize * blockSize) + (blockIndexY * blockSize) + blockIndexX];
      }
    }
  }
  // Apply empirical Wiener filtering to the source group guided by the reference group

  const auto finalAMP = static_cast<float>(2 * Group_Size_Final * 2 * BLOCK_SIZE * 2 * BLOCK_SIZE);
  const float forwardAMP = sqrt(finalAMP);
  const FLType sigmaSquare = sigma * forwardAMP * sigma * forwardAMP;

  auto* srcp = srcGroup.data();
  auto* refp = refGroup.data();
  auto* const upper = srcp + srcGroup.size();

#if defined(__SSE2__)
  static const ptrdiff_t simd_step = 4;
  const auto simd_residue = static_cast<ptrdiff_t>(srcGroup.size() % simd_step);
  const ptrdiff_t simd_width = static_cast<ptrdiff_t>(srcGroup.size()) - simd_residue;

  const __m128 sgm_sqr = _mm_set_ps1(sigmaSquare);
  __m128 l2wiener_sum = _mm_setzero_ps();

  for (auto* const upper1 = srcp + simd_width; srcp < upper1; srcp += simd_step, refp += simd_step) {
    const __m128 srcVec = _mm_load_ps(srcp);
    const __m128 refVec = _mm_load_ps(refp);
    const __m128 r1sqr = _mm_mul_ps(refVec, refVec);

    const __m128 wiener = _mm_mul_ps(r1sqr, _mm_rcp_ps(_mm_add_ps(r1sqr, sgm_sqr)));

    const __m128 resultVec = _mm_mul_ps(srcVec, wiener);
    _mm_store_ps(srcp, resultVec);
    l2wiener_sum = _mm_add_ps(l2wiener_sum, _mm_mul_ps(wiener, wiener));
  }

  alignas(16) std::array<FLType, 4> l2wiener_sum_f32 = {};
  _mm_store_ps(l2wiener_sum_f32.data(), l2wiener_sum);
  L2Wiener += l2wiener_sum_f32[0] + l2wiener_sum_f32[1] + l2wiener_sum_f32[2] + l2wiener_sum_f32[3];
#endif

  for (; srcp < upper; ++srcp, ++refp) {
    const FLType refSquare = *refp * *refp;
    const FLType wienerCoef = refSquare / (refSquare + sigmaSquare);
    *srcp *= wienerCoef;
    L2Wiener += wienerCoef * wienerCoef;
  }

  // Apply backward 3D transform to the filtered group

  // Backward 3D DCT for the filtered group
  for (int grpIndex = 0; grpIndex < grpSize; ++grpIndex) {
    for (int blockIndexY = 0; blockIndexY < blockSize; ++blockIndexY) {
      for (int blockIndexX = 0; blockIndexX < blockSize; ++blockIndexX) {
        data[(grpIndex * blockSize * blockSize) + (blockIndexY * blockSize) + blockIndexX] =
            srcGroup(grpIndex, blockIndexY, blockIndexX);
      }
    }
  }
  fftwf_execute(planCache.getInversePlan(grpSize));
  for (int grpIndex = 0; grpIndex < grpSize; ++grpIndex) {
    for (int blockIndexY = 0; blockIndexY < blockSize; ++blockIndexY) {
      for (int blockIndexX = 0; blockIndexX < blockSize; ++blockIndexX) {
        srcGroup(grpIndex, blockIndexY, blockIndexX) =
            data[(grpIndex * blockSize * blockSize) + (blockIndexY * blockSize) + blockIndexX];
      }
    }
  }

  // Calculate weight for the filtered group
  // Also include the normalization factor to compensate for the amplification introduced in 3D transform
  L2Wiener = Max(std::numeric_limits<float>::epsilon(), L2Wiener);
  const FLType denWeight = static_cast<FLType>(1) / L2Wiener;
  const auto numWeight = static_cast<FLType>(denWeight / finalAMP);

  // Store the weighted filtered group to the numerator part of the final estimation
  // Store the weight to the denominator part of the final estimation
  srcGroup.AddTo(ResNum, static_cast<PCType>(width), numWeight);
  srcGroup.CountTo(ResDen, static_cast<PCType>(width), denWeight);
}

int DenoiseProcessor::nonLocalMeans(int* inputBuffer, int* outputBuffer, int width, int height) {
  const float hSquared = SIGMA_H * SIGMA_H;
  int result = 0;
#pragma omp parallel for schedule(dynamic)  // Parallelizing outer loop
  for (int row = 0; row < height; row++) {
    for (int col = 0; col < width; col++) {
      double weightedSum = 0.0;
      double totalWeight = 0.0;
      const int pixelIndex = (row * width) + col;
      // Loop over the search window
      for (int dy = -WORKING_RADIUS; dy <= WORKING_RADIUS; dy++) {
        for (int dx = -WORKING_RADIUS; dx <= WORKING_RADIUS; dx++) {
          const int currentCol = col + dx;
          const int currentRow = row + dy;

          // Check if within bounds
          if (currentCol >= 0 && currentCol < width && currentRow >= 0 && currentRow < height) {
            const int neighborIndex = (currentRow * width) + currentCol;
            // Calculate patch distance
            const float dist = calculatePatchDistance(inputBuffer, col, row, currentCol, currentRow, width, height);
            const float weight = expf(-dist / hSquared);
            // Weighted sum
            weightedSum += weight * static_cast<float>(inputBuffer[neighborIndex]);
            totalWeight += weight;
          }
        }
      }
      // Normalize and assign to output image
      result = static_cast<int>(weightedSum / totalWeight);
      outputBuffer[pixelIndex] = result;
    }
  }
  return 1;
}

float DenoiseProcessor::calculatePatchDistance(const int* inputBuffer, int col, int row, int currentCol, int currentRow,
                                               int width, int height) {
  float distance = 0.0;

#pragma omp simd reduction(+ : distance)  // Vectorize loop for better performance
  for (int dy = -SEARCH_RADIUS; dy <= SEARCH_RADIUS; dy++) {
    for (int dx = -SEARCH_RADIUS; dx <= SEARCH_RADIUS; dx++) {
      const int sourceCol = col + dx;
      const int sourceRow = row + dy;
      const int targetCol = currentCol + dx;
      const int targetRow = currentRow + dy;

      // Ensure coordinates are within bounds
      if (sourceCol >= 0 && sourceCol < width && sourceRow >= 0 && sourceRow < height && targetCol >= 0 &&
          targetCol < width && targetRow >= 0 && targetRow < height) {
        const int index = (sourceRow * width) + sourceCol;
        const int indexn = (targetRow * width) + targetCol;

        const float diff = static_cast<float>(inputBuffer[index]) - static_cast<float>(inputBuffer[indexn]);
        distance = distance + (diff * diff);
      }
    }
  }
  return sqrtf(distance);
}

int DenoiseProcessor::WaveletDenoise(float* outputBuffer, const float* inputBuffer, int width, int height) {
  const int overlap = OVERLAP;
  const int blockSize = PADDING;
  const int padding = blockSize;
  // Dimensions for the padded image
  const int paddedWidth = width + (2 * padding);
  const int paddedHeight = height + (2 * padding);

// Copy original image into the center of the padded image
#pragma omp parallel for simd collapse(2)
  for (int row = 0; row < height; ++row) {
    for (int col = 0; col < width; ++col) {
      this->paddedImage[((row + padding) * paddedWidth) + (col + padding)] = inputBuffer[(row * width) + col];
    }
  }

// Mirror edges
#pragma omp parallel for simd collapse(2)
  for (int row = 0; row < padding; ++row) {
    for (int col = 0; col < paddedWidth; ++col) {
      // Top and bottom edges
      this->paddedImage[(row * paddedWidth) + col] =
          this->paddedImage[((2 * padding - row) * paddedWidth) + col];  // Top
      this->paddedImage[((paddedHeight - row - 1) * paddedWidth) + col] =
          this->paddedImage[((paddedHeight - 2 * padding + row - 1) * paddedWidth) + col];  // Bottom
    }
  }

// Mirror sides
#pragma omp parallel for simd collapse(2)
  for (int row = 0; row < paddedHeight; ++row) {
    for (int col = 0; col < padding; ++col) {
      this->paddedImage[(row * paddedWidth) + col] =
          this->paddedImage[(row * paddedWidth) + (2 * padding) - col];  // Left
      this->paddedImage[(row * paddedWidth) + paddedWidth - col - 1] =
          this->paddedImage[(row * paddedWidth) + paddedWidth - (2 * padding) + col - 1];  // Right
    }
  }

  // Perform DWT denoising on the temp buffer
  dwtDenoise(this->paddedImage.data(), paddedWidth, paddedHeight);
#if __GNUC__ >= 10
#pragma omp parallel for simd collapse(2)
#else
#pragma omp parallel for collapse(2)
#endif
  for (int row = 0; row <= paddedHeight - blockSize; row += 1) {
    for (int col = 0; col <= paddedWidth - blockSize; col += 1) {
      int sum = 0;
      const int stIndex = (blockSize - overlap) / 2;
      const int endIndex = stIndex + overlap;

#pragma omp simd reduction(+ : sum)  // SIMD for better memory access
      for (int by = stIndex; by < endIndex; by++) {
        for (int bx = stIndex; bx < endIndex; bx++) {
          sum += this->paddedImage[((row + by) * paddedWidth) + (col + bx)];
        }
      }
      this->paddedImageOut[(row * paddedWidth) + col] = sum / (overlap * overlap);
    }
  }

#pragma omp parallel for simd collapse(2)
  for (int row = padding - (blockSize / 2); row < height + padding - blockSize / 2; row++) {
    for (int col = padding - (blockSize / 2); col < width + padding - blockSize / 2; col++) {
      const int targetRow = row - (padding - (blockSize / 2));  // Compute i separately
      const int targetCol = col - (padding - (blockSize / 2));  // Compute j separately
      outputBuffer[(targetRow * width) + targetCol] = this->paddedImageOut[(row * paddedWidth) + col];
    }
  }

  return 1;  // Success
}

int DenoiseProcessor::waveletTransform(double* block, int width, int height) {
  const int levels = LEVEL;

  int low = 0;
  int high = 0;
  for (int level = 0; level < levels; level++) {
    const int currentWidth = width >> level;    // Width for current level
    const int currentHeight = height >> level;  // Height for current level

// Transform Rows
// Parallel Transform Rows
#pragma omp parallel for simd collapse(2) private(low, high)
    for (int row = 0; row < currentHeight; row++) {
      for (int index = 0; index < currentWidth / 2; index++) {
        low = static_cast<int>(block[(row * width) + (2 * index)]);
        high = static_cast<int>(block[(row * width) + (2 * index) + 1]);
        this->transformTemp[(row * width) + index] = (low + high) / TRFMCONST;
        this->transformTemp[(row * width) + (currentWidth / 2) + index] = (low - high) / TRFMCONST;
      }
    }

// Transform Columns
// Parallel Transform Columns
#pragma omp parallel for simd collapse(2) private(low, high)
    for (int col = 0; col < currentWidth; col++) {
      for (int index = 0; index < currentHeight / 2; index++) {
        low = this->transformTemp[(2 * index * width) + col];
        high = this->transformTemp[((2 * index + 1) * width) + col];
        block[(index * width) + col] = static_cast<double>(low + high) / TRFMCONST;
        block[((currentHeight / 2 + index) * width) + col] = static_cast<double>(low - high) / TRFMCONST;
      }
    }

#if DEBUG == 1
    if (blockNumber == BLOCKNO) {
      //printf("DWT L%d\n", level);
      display_2d_block(block, width, height);
      //printf("\n\n");
    }
#endif  // DEBUG
  }
  return 1;
}

int DenoiseProcessor::inverseWaveletTransform(double* block, int width, int height) {
  const int levels = LEVEL;

  int low = 0;
  int high = 0;
  for (int level = levels - 1; level >= 0; level--) {
    const int currentWidth = width >> level;    // Reduce width for the current level
    const int currentHeight = height >> level;  // Reduce height for the current level

// Inverse Transform Columns
#pragma omp parallel for simd collapse(2) private(low, high)
    for (int col = 0; col < currentWidth; col++) {
      for (int index = 0; index < currentHeight / 2; index++) {
        low = static_cast<int>(block[(index * width) + col]);
        high = static_cast<int>(block[((currentHeight / 2 + index) * width) + col]);

        this->inverseTransformTemp[(2 * index * width) + col] = (low + high) * INVTRFMCONST;
        this->inverseTransformTemp[((2 * index + 1) * width) + col] = (low - high) * INVTRFMCONST;
      }
    }

// Inverse Transform Rows
#pragma omp parallel for simd collapse(2) private(low, high)
    for (int row = 0; row < currentHeight; row++) {
      for (int index = 0; index < currentWidth / 2; index++) {
        low = this->inverseTransformTemp[(row * width) + index];
        high = this->inverseTransformTemp[(row * width) + (currentWidth / 2) + index];

        block[(row * width) + (2 * index)] = (static_cast<double>(low) + high) * INVTRFMCONST;
        block[(row * width) + (2 * index) + 1] = (static_cast<double>(low) - high) * INVTRFMCONST;
      }
    }

#if DEBUG == 1
    if (blockNumber == BLOCKNO) {
      //printf("IDWT L%d\n", level);
      display_2d_block(block, width, height);
      //printf("\n\n");
    }
#endif  // DEBUG
  }

  return 1;
}

int DenoiseProcessor::dwtDenoise(float* inputBuffer, int width, int height) {
  const int size = width * height;

#pragma omp parallel for simd
  for (int i = 0; i < size; i++) {
    this->coeffs[i] = static_cast<double>(inputBuffer[i]);
  }

  #pragma omp parallel for simd
  for (int i = 0; i < size; i++) {
    this->coeffs[i] = this->coeffs[i] - 128.0;  // Centering the data
  }

  /* Transformation */
  waveletTransform(this->coeffs.data(), width, height);
  /* Thresholding*/
  applyLevelDependentThresholding(this->coeffs.data(), width, height);
  /* Inverse Transformation */
  inverseWaveletTransform(this->coeffs.data(), width, height);

  #pragma omp parallel for simd
  for (int i = 0; i < size; i++) {
    this->coeffs[i] = std::min(255.0, std::max(0.0, this->coeffs[i] + 128.0));
  }

#pragma omp parallel for simd
  for (int i = 0; i < size; i++) {
    inputBuffer[i] = static_cast<float>(this->coeffs[i]);
  }

  return 1;
}

// Comparator function for qsort
int DenoiseProcessor::compareDouble(const void* valueA, const void* valueB) {
  const double diff = *reinterpret_cast<const double*>(valueA) - *reinterpret_cast<const double*>(valueB);
  return static_cast<int>(diff > 0) - static_cast<int>(diff < 0);  // Returns -1, 0, or 1
}

// Function to calculate the median of an array
double DenoiseProcessor::findMedian(double* arr, int size) {
  if (size == 1) {
    return arr[0];
  }
  qsort(arr, size, sizeof(double), compareDouble);
  if (size % 2 == 0) {
    // Average of the two middle values for even-sized arrays
    return (arr[(size / 2) - 1] + arr[size / 2]) / 2.0;
  } else {
    // Middle value for odd-sized arrays
    return arr[size / 2];
  }
}

void DenoiseProcessor::applyLevelDependentThresholding(double* coeffs, int width, int height) {
  int currentWidth = width;
  int currentHeight = height;
  int blockSize = BLOCKSIZE;
  const int levels = LEVEL;
  double varianceHH = 0;
  double varianceHL = 0;
  double varianceLH = 0;
  double thresholdHL = 0;
  double thresholdLH = 0;
  double thresholdHH = 0;
  // #pragma omp parallel for schedule(dynamic)
  for (int level = 0; level < levels; level++) {
    const int detailWidth = currentWidth / 2;
    const int detailHeight = currentHeight / 2;
    const int bandSize = blockSize * blockSize;

    const double levelScale = sqrt(2.0 * log(bandSize));
    const int thresh = THRESHOLD * levelScale;

// Parallelizing expensive operations
#pragma omp parallel sections
    {
#pragma omp section
      {
        // Sub-band pointers for LH
        double* LHBand = coeffs + (static_cast<ptrdiff_t>(detailHeight) * width);  // LH starts after LL vertically

        if (blockSize == 1) {
          varianceLH = estimateNoiseVariance(LHBand, detailWidth, detailHeight, width);
          thresholdLH = varianceLH;
        } else {
          thresholdLH = static_cast<double>(thresh);
        }
        applySoftThresholding(LHBand, detailWidth, detailHeight, width, thresholdLH);
      }
#pragma omp section
      {
        // Sub-band pointers for HL
        double* HLBand = coeffs + (detailWidth);  // HL starts after LL horizontally

        if (blockSize == 1) {
          varianceHL = estimateNoiseVariance(HLBand, detailWidth, detailHeight, width);
          thresholdHL = varianceHL;
        } else {
          thresholdHL = static_cast<double>(thresh);
        }
        applySoftThresholding(HLBand, detailWidth, detailHeight, width, thresholdHL);
      }
#pragma omp section
      {
        // Sub-band pointers for HH
        double* HHBand = coeffs + (static_cast<ptrdiff_t>(detailHeight) * width) + detailWidth;  // HH after HL

        if (blockSize == 1) {
          varianceHH = estimateNoiseVariance(HHBand, detailWidth, detailHeight, width);
          thresholdHH = varianceHH;
        } else {
          thresholdHH = static_cast<double>(thresh);
        }
        applySoftThresholding(HHBand, detailWidth, detailHeight, width, thresholdHH);
      }
    }

    // Update dimensions for the next level
    currentWidth = detailWidth;
    currentHeight = detailHeight;
    blockSize = blockSize / 2;
  }
}

double DenoiseProcessor::estimateNoiseVariance(double* coeffs, int width, int height, int stride) {
  const int total = width * height;

// Flatten coefficients into a single array for median calculation
#pragma omp parallel for simd collapse(2)
  for (int row = 0; row < height; row++) {
    for (int col = 0; col < width; col++) {
      this->values[(row * width) + col] = fabs(coeffs[(row * stride) + col]);
    }
  }
  // Step 1: Find the median of the coefficients
  const double median = findMedian(this->values.data(), total);

  // Step 2: Scale MAD to estimate standard deviation
  return median / 0.6745;
}

// Apply soft thresholding
void DenoiseProcessor::applySoftThresholding(double* coeffs, int width, int height, int stride, double threshold) {
#pragma omp parallel for simd collapse(2)
  for (int row = 0; row < height; row++) {
    for (int col = 0; col < width; col++) {
      const double value = coeffs[(row * stride) + col];
      coeffs[(row * stride) + col] = (fabs(value) > threshold) ? (fabs(value) - threshold) * (value > 0 ? 1 : -1) : 0;
    }
  }
}
