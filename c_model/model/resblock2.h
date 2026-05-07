#ifndef RESBLOCK2_H
#define RESBLOCK2_H

#include <stdint.h>

void resblock2_forward(
    const uint8_t *input,
    uint8_t *output,

    uint8_t *buf1,
    uint8_t *buf2,

    const int8_t *conv1_weight,
    const int32_t *conv1_bias,
    const int32_t *conv1_multiplier,
    const int32_t *conv1_shift,
    const int32_t *conv1_weight_zp,

    const int8_t *conv2_weight,
    const int32_t *conv2_bias,
    const int32_t *conv2_multiplier,
    const int32_t *conv2_shift,
    const int32_t *conv2_weight_zp,

    int32_t conv1_Zx,
    int32_t conv1_Zy,
    int32_t conv2_Zx,
    int32_t conv2_Zy,

    float S_main,
    float S_skip,
    float S_add,

    int32_t Z_main,
    int32_t Z_skip,
    int32_t Z_add,

    int32_t relu_Zy
);

#endif