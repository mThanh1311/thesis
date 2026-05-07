#include <stdint.h>
#include "downsample.h"

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

#define DOWN_IN_CH   32
#define DOWN_IN_LEN  160
#define DOWN_OUT_CH  64
#define DOWN_OUT_LEN 80
#define DOWN_KERNEL  5
#define DOWN_STRIDE  2
#define DOWN_PADDING 2
#define DOWN_SIZE    (DOWN_OUT_CH * DOWN_OUT_LEN)

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
) {
    static uint8_t conv_out[DOWN_SIZE];

    conv1d_ref(
        input,
        weight,
        bias,
        conv_out,
        DOWN_IN_CH,
        DOWN_OUT_CH,
        DOWN_IN_LEN,
        DOWN_KERNEL,
        DOWN_STRIDE,
        DOWN_PADDING,
        multiplier,
        shift,
        conv_Zx,
        weight_zp,
        conv_Zy
    );

    relu_ref(conv_out, output, DOWN_SIZE, relu_Zy);
}