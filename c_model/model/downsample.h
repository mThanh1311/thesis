#ifndef DOWNSAMPLE_H
#define DOWNSAMPLE_H

#include <stdint.h>

void downsample_forward(
    const uint8_t *input,
    uint8_t *output,

    const int8_t *weight,
    const int32_t *bias,
    const int32_t *multiplier,
    const int32_t *shift,
    const int32_t *weight_zp,

    int32_t conv_Zx,
    int32_t conv_Zy,
    int32_t relu_Zy
);

#endif