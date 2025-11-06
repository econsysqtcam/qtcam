#include "Denoise.hpp"

#include "DenoiseCls.hpp"

Denoise::Denoise() : denoiseProcessorInstance(std::make_unique<DenoiseProcessor>()) {}

Denoise::~Denoise() = default;

int Denoise::DenoiseYUV(float* output_buffer, float* input_buffer, int width, int height, float gain_val, float denoising_strength) {
  return denoiseProcessorInstance->DenoiseYUV(output_buffer, input_buffer, width, height, gain_val, denoising_strength);
}
