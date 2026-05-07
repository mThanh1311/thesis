#include <stdint.h>

#include "cnn.h"
#include "stem.h"
#include "resblock.h"
#include "downsample.h"
#include "resblock2.h"

#define RES1_SIZE (32 * 160)
#define RES2_SIZE (64 * 80)

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
) {
    static uint8_t stem_out[RES1_SIZE];
    static uint8_t res1_out[RES1_SIZE];

    static uint8_t res1_buf1[RES1_SIZE];
    static uint8_t res1_buf2[RES1_SIZE];

    static uint8_t down_out[RES2_SIZE];

    static uint8_t res2_out[RES2_SIZE];
    static uint8_t res2_buf1[RES2_SIZE];
    static uint8_t res2_buf2[RES2_SIZE];

    // ===== STEM =====
    stem_forward(
        input,
        stem_out,

        stem_w,
        stem_b,
        stem_m,
        stem_s,
        stem_zw,

        stem_Zx,
        stem_Zy,
        stem_relu_Zy
    );

    // ===== RES1 =====
    resblock_forward(
        stem_out,
        res1_out,

        res1_buf1,
        res1_buf2,

        res1_w1,
        res1_b1,
        res1_m1,
        res1_s1,
        res1_zw1,

        res1_w2,
        res1_b2,
        res1_m2,
        res1_s2,
        res1_zw2,

        res1_conv1_Zx,
        res1_conv1_Zy,

        res1_conv2_Zx,
        res1_conv2_Zy,

        res1_S_main,
        res1_S_skip,
        res1_S_add,

        res1_Z_main,
        res1_Z_skip,
        res1_Z_add,

        res1_relu_Zy
    );

    // ===== DOWN1 =====
    downsample_forward(
        res1_out,
        down_out,

        down_w,
        down_b,
        down_m,
        down_s,
        down_zw,

        down_Zx,
        down_Zy,
        down_relu_Zy
    );

    // ===== RES2 =====
    resblock2_forward(
        down_out,
        res2_out,

        res2_buf1,
        res2_buf2,

        res2_w1,
        res2_b1,
        res2_m1,
        res2_s1,
        res2_zw1,

        res2_w2,
        res2_b2,
        res2_m2,
        res2_s2,
        res2_zw2,

        res2_conv1_Zx,
        res2_conv1_Zy,

        res2_conv2_Zx,
        res2_conv2_Zy,

        res2_S_main,
        res2_S_skip,
        res2_S_add,

        res2_Z_main,
        res2_Z_skip,
        res2_Z_add,

        res2_relu_Zy
    );

    for (int i = 0; i < RES2_SIZE; i++) {
        output[i] = res2_out[i];
    }
}