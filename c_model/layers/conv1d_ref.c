#include <stdint.h>
#include "quant.h"

void conv1d_ref (
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
) {
    int out_len = (in_len + 2 * padding - kernel) / stride + 1;

    for (int oc = 0; oc < out_ch; oc++) {
        for (int ol = 0; ol < out_len; ol++) {

            int32_t acc = bias[oc];

            for (int ic = 0; ic < in_ch; ic++) {
                for (int k = 0; k < kernel; k++) {

                    int il = ol * stride + k - padding;

                    uint8_t xq;

                    if (il < 0 || il >= in_len) {
                        xq = (uint8_t)Zx;
                    } else {
                        xq = input[ic * in_len + il];
                    }

                    int32_t x_centered = (int32_t)xq - Zx;

                    int8_t wq = weight[k * in_ch * out_ch + ic * out_ch + oc];
                    int32_t w_centered = (int32_t)wq - Zw[oc];

                    acc += x_centered * w_centered;
                }
            }

            // Requant
            int64_t scaled = (int64_t)acc * (int64_t)multiplier[oc];

            int total_shift = 31 + shift[oc];

            // rounding to nearest
            if (total_shift > 0) {
                int64_t rounding = 1LL << (total_shift - 1);

                if (scaled >= 0)
                    scaled += rounding;
                else
                    scaled -= rounding;
            }

            int32_t q = (int32_t)(scaled >> total_shift) + Zy;
            
            // clamp uint8
            if (q < 0) q = 0;
            if (q > 255) q = 255;

            output[oc * out_len + ol] = (uint8_t)q;
        }
    }
}