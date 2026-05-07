#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include "quant.h"

float dequant(int8_t q, float s, int32_t z) {
    return s * ((int32_t)q - z);
}

int8_t quant(float r, float s, int32_t z) {
    int32_t q = (int32_t)roundf(r / s) + z;
    return clamp_int8(q);
}

int8_t clamp_int8(int32_t x){
    if (x > 127) {
        return 127;
    } else if (x < -128) {
        return -128;
    } else {
        return (int8_t)x;
    }
}

int8_t requantize(
    int32_t acc,
    float sx,
    float sy,
    float sz,
    int32_t zy
) {
    float m = sx * sy / sz;
    int32_t q = (int32_t)roundf(acc * m) + zy;
    return clamp_int8(q);
}

void quantize_multiplier(
    float real_scale,
    int32_t *multiplier,
    int32_t *shift
) {
    if (real_scale == 0.0f) {
        *multiplier = 0;
        *shift = 0;
        return;
    }

    int s;
    double q = frexp((double)real_scale, &s);

    int64_t m = (int64_t)round(q * (1LL << 31));

    if (m == (1LL << 31)) {
        m /= 2;
        s++;
    }

    *multiplier = (int32_t)m;
    *shift = s;
}