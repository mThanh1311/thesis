#ifndef QUANT_H
#define QUANT_H

#include <math.h>
#include <stdint.h>

float dequant(int8_t q, float s, int32_t z);

int8_t quant(float r, float s, int32_t z);

int8_t requantize(
    int32_t acc,
    float sx,
    float sy,
    float sz,
    int32_t zy
);

int8_t clamp_int8(int32_t x);

void quantize_multiplier(
    float real_scale,
    int32_t *multiplier,
    int32_t *shift
);

#endif // QUANT_H