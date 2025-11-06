#include <stdint.h>

#include <memory>

class DenoiseProcessor;  // Forward declaration
class Denoise {
 private:
  std::unique_ptr<DenoiseProcessor> denoiseProcessorInstance;

 public:
  Denoise();
  ~Denoise();
  int DenoiseYUV(float* output_buffer, float* input_buffer, int width, int height, float gain_val, float denoising_strength);
};
