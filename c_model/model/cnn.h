#ifndef CNN_H
#define CNN_H

#include <stdint.h>

void cnn_forward(
    const uint8_t *input,
    uint8_t *output,

    // ===== STEM =====
    const int8_t *stem_w,
    const int32_t *stem_b,
    const int32_t *stem_m,
    const int32_t *stem_s,
    const int32_t *stem_zw,
    int32_t stem_Zx,
    int32_t stem_Zy,
    int32_t stem_relu_Zy,

    // ===== RES1 CONV1 =====
    const int8_t *res1_w1,
    const int32_t *res1_b1,
    const int32_t *res1_m1,
    const int32_t *res1_s1,
    const int32_t *res1_zw1,

    // ===== RES1 CONV2 =====
    const int8_t *res1_w2,
    const int32_t *res1_b2,
    const int32_t *res1_m2,
    const int32_t *res1_s2,
    const int32_t *res1_zw2,

    int32_t res1_conv1_Zx,
    int32_t res1_conv1_Zy,
    int32_t res1_conv2_Zx,
    int32_t res1_conv2_Zy,

    float res1_S_main,
    float res1_S_skip,
    float res1_S_add,

    int32_t res1_Z_main,
    int32_t res1_Z_skip,
    int32_t res1_Z_add,

    int32_t res1_relu_Zy,

    // ===== DOWN1 =====
    const int8_t *down_w,
    const int32_t *down_b,
    const int32_t *down_m,
    const int32_t *down_s,
    const int32_t *down_zw,
    int32_t down_Zx,
    int32_t down_Zy,
    int32_t down_relu_Zy,

    // ===== RES2 CONV1 =====
    const int8_t *res2_w1,
    const int32_t *res2_b1,
    const int32_t *res2_m1,
    const int32_t *res2_s1,
    const int32_t *res2_zw1,

    // ===== RES2 CONV2 =====
    const int8_t *res2_w2,
    const int32_t *res2_b2,
    const int32_t *res2_m2,
    const int32_t *res2_s2,
    const int32_t *res2_zw2,

    int32_t res2_conv1_Zx,
    int32_t res2_conv1_Zy,
    int32_t res2_conv2_Zx,
    int32_t res2_conv2_Zy,

    float res2_S_main,
    float res2_S_skip,
    float res2_S_add,

    int32_t res2_Z_main,
    int32_t res2_Z_skip,
    int32_t res2_Z_add,

    int32_t res2_relu_Zy
);

#endif