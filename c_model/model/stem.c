#include <stdint.h>
#include "stem.h"

// ===== Layer declarations =====
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

// ===== Stem constants =====
#define STEM_IN_CH   1
#define STEM_IN_LEN  320
#define STEM_OUT_CH  32
#define STEM_KERNEL  5
#define STEM_STRIDE  2
#define STEM_PADDING 2
#define STEM_OUT_LEN 160
#define STEM_OUT_SIZE (STEM_OUT_CH * STEM_OUT_LEN)

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
) {
    static uint8_t conv_out[STEM_OUT_SIZE];

    conv1d_ref(
        input,
        conv_weight,
        conv_bias,
        conv_out,

        STEM_IN_CH,
        STEM_OUT_CH,
        STEM_IN_LEN,
        STEM_KERNEL,
        STEM_STRIDE,
        STEM_PADDING,

        conv_multiplier,
        conv_shift,

        conv_Zx,
        conv_weight_zp,
        conv_Zy
    );

    relu_ref(
        conv_out,
        output,
        STEM_OUT_SIZE,
        relu_Zy
    );
}