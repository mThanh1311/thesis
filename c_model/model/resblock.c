#include <stdint.h>
#include "resblock.h"

void conv1d_ref(
    const uint8_t *input,
    const int8_t *weight,
    const int32_t *bias,
    uint8_t *output,
    int in_ch,
    int out_ch,
    int in_len,
    int kernel,
    int stride,
    int padding,
    const int32_t *multiplier,
    const int32_t *shift,
    int32_t Zx,
    const int32_t *Zw,
    int32_t Zy
);

void relu_ref(
    const uint8_t *input,
    uint8_t *output,
    int size,
    int32_t Zy
);

void add_ref(
    const uint8_t *x1,
    const uint8_t *x2,
    uint8_t *y,
    int size,
    float S1,
    float S2,
    float Sy,
    int32_t Z1,
    int32_t Z2,
    int32_t Zy
);

#define RES_CH      32
#define RES_LEN     160
#define RES_SIZE    (RES_CH * RES_LEN)

#define RES_KERNEL  5
#define RES_STRIDE  1
#define RES_PADDING 2

void resblock_forward(
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
) {
    // Conv1
    conv1d_ref(
        input,
        conv1_weight,
        conv1_bias,
        buf1,
        RES_CH,
        RES_CH,
        RES_LEN,
        RES_KERNEL,
        RES_STRIDE,
        RES_PADDING,
        conv1_multiplier,
        conv1_shift,
        conv1_Zx,
        conv1_weight_zp,
        conv1_Zy
    );

    // ReLU after Conv1
    relu_ref(buf1, buf1, RES_SIZE, conv1_Zy);

    // Conv2
    conv1d_ref(
        buf1,
        conv2_weight,
        conv2_bias,
        buf2,
        RES_CH,
        RES_CH,
        RES_LEN,
        RES_KERNEL,
        RES_STRIDE,
        RES_PADDING,
        conv2_multiplier,
        conv2_shift,
        conv2_Zx,
        conv2_weight_zp,
        conv2_Zy
    );

    // Add: main branch + skip branch
    add_ref(
        buf2,
        input,
        buf1,
        RES_SIZE,
        S_main,
        S_skip,
        S_add,
        Z_main,
        Z_skip,
        Z_add
    );

    // Final ReLU
    relu_ref(buf1, output, RES_SIZE, relu_Zy);
}