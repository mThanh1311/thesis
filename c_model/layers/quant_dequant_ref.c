#include <stdint.h>
#include <math.h>

void quantize_fp32_to_u8(
    const float *input,
    uint8_t *output,
    int size,
    float S,
    int32_t Z
) {
    for (int i = 0; i < size; i++) {
        int32_t q = (int32_t)roundf(input[i] / S) + Z;

        if (q < 0) q = 0;
        if (q > 255) q = 255;

        output[i] = (uint8_t)q;
    }
}

void dequantize_u8_to_fp32(
    const uint8_t *input,
    float *output,
    int size,
    float S,
    int32_t Z
) {
    for (int i = 0; i < size; i++) {
        output[i] = S * ((int32_t)input[i] - Z);
    }
}