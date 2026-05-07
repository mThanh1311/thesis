#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "conv1d_hls.h"

static void conv1d_golden(
    const qact_t input[STEM_IN_SIZE],
    const qwgt_t weight[STEM_W_SIZE],
    const qbias_t bias[STEM_B_SIZE],
    qact_t output[STEM_OUT_SIZE],

    const qmult_t multiplier[STEM_OUT_CH],
    const qshift_t shift[STEM_OUT_CH],
    const qzp_t weight_zp[STEM_OUT_CH],

    qzp_t Zx,
    qzp_t Zy
) {
    for (int oc = 0; oc < STEM_OUT_CH; oc++) {
        for (int ol = 0; ol < STEM_OUT_LEN; ol++) {

            qacc_t acc = bias[oc];

            for (int ic = 0; ic < STEM_IN_CH; ic++) {
                for (int k = 0; k < STEM_KERNEL; k++) {

                    int il = ol * STEM_STRIDE + k - STEM_PADDING;

                    qact_t xq;

                    if (il < 0 || il >= STEM_IN_LEN)
                        xq = (qact_t)Zx;
                    else
                        xq = input[ic * STEM_IN_LEN + il];

                    qacc_t x_centered = (qacc_t)xq - Zx;

                    qwgt_t wq = weight[
                        k * STEM_IN_CH * STEM_OUT_CH +
                        ic * STEM_OUT_CH +
                        oc
                    ];

                    qacc_t w_centered = (qacc_t)wq - weight_zp[oc];

                    acc += x_centered * w_centered;
                }
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

            qacc_t q = (qacc_t)(scaled >> total_shift) + Zy;

            if (q < 0) q = 0;
            if (q > 255) q = 255;

            output[oc * STEM_OUT_LEN + ol] = (qact_t)q;
        }
    }
}

int main() {
    static qact_t input[STEM_IN_SIZE];
    static qwgt_t weight[STEM_W_SIZE];
    static qbias_t bias[STEM_B_SIZE];
    static qact_t expected[STEM_OUT_SIZE];
    static qact_t actual[STEM_OUT_SIZE];

    static qmult_t multiplier[STEM_OUT_CH];
    static qshift_t shift[STEM_OUT_CH];
    static qzp_t weight_zp[STEM_OUT_CH];

    qzp_t Zx = 128;
    qzp_t Zy = 127;

    for (int i = 0; i < STEM_IN_SIZE; i++) {
        input[i] = (qact_t)((i * 7 + 13) % 256);
    }

    for (int i = 0; i < STEM_W_SIZE; i++) {
        weight[i] = (qwgt_t)((i * 5) % 17 - 8);
    }

    for (int oc = 0; oc < STEM_OUT_CH; oc++) {
        bias[oc] = oc - 16;

        // multiplier/shift synthetic.
        // Dùng giá trị an toàn để test arithmetic.
        multiplier[oc] = 1073741824; // approx 0.5 in Q31
        shift[oc] = 0;

        weight_zp[oc] = 0;
    }

    conv1d_golden(
        input,
        weight,
        bias,
        expected,
        multiplier,
        shift,
        weight_zp,
        Zx,
        Zy
    );

    stem_conv1d_hls(
        input,
        weight,
        bias,
        actual,
        multiplier,
        shift,
        weight_zp,
        Zx,
        Zy
    );

    int max_diff = 0;
    int mismatch = 0;

    for (int i = 0; i < STEM_OUT_SIZE; i++) {
        int diff = (int)actual[i] - (int)expected[i];
        if (diff < 0) diff = -diff;

        if (diff > max_diff) max_diff = diff;

        if (diff != 0) {
            mismatch++;
            if (mismatch < 20) {
                printf(
                    "Mismatch i=%d expected=%u actual=%u diff=%d\n",
                    i,
                    expected[i],
                    actual[i],
                    diff
                );
            }
        }
    }

    printf("Running Conv1D HLS test...\n");

    printf(
        "input[0]=%u weight[0]=%d bias[0]=%d\n",
        input[0],
        weight[0],
        bias[0]
    );

    printf("Conv1D HLS test: max_diff=%d mismatch=%d\n", max_diff, mismatch);

    if (mismatch == 0) {
        printf("PASS\n");
        return 0;
    } else {
        printf("FAIL\n");
        return 1;
    }
}