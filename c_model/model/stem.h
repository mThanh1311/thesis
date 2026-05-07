#ifndef STEM_H
#define STEM_H

#include <stdint.h>

void stem_forward(
    const uint8_t *input,
    uint8_t *output,

    const int8_t *conv_weight,
    const int32_t *conv_bias,
    const int32_t *conv_multiplier,
    const int32_t *conv_shift,
    const int32_t *conv_weight_zp,

    int32_t conv_Zx,
    int32_t conv_Zy,
    int32_t relu_Zy
);

#endif