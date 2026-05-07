#include <stdint.h>
#include <math.h>

void fc_ref(
    const uint8_t *input,
    const int8_t *weight,
    const int32_t *bias,
    uint8_t *output,

    int in_features,
    int out_features,

    const int32_t *multiplier,
    const int32_t *shift,

    int32_t Zx,
    const int32_t *Zw,
    int32_t Zy
) {
    for (int oc = 0; oc < out_features; oc++) {

        int32_t acc = bias[oc];

        for (int i = 0; i < in_features; i++) {
            int32_t x = (int32_t)input[i] - Zx;
            int32_t w = (int32_t)weight[oc * in_features + i] - Zw[oc];

            acc += x * w;
        }

        int64_t scaled = (int64_t)acc * (int64_t)multiplier[oc];
        int total_shift = 31 + shift[oc];

        if (total_shift > 0) {
            int64_t rounding = 1LL << (total_shift - 1);

            if (scaled >= 0)
                scaled += rounding;
            else
                scaled -= rounding;
        }

        int32_t q = (int32_t)(scaled >> total_shift) + Zy;

        if (q < 0) q = 0;
        if (q > 255) q = 255;

        output[oc] = (uint8_t)q;
    }
}