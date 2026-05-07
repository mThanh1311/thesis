#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "add_hls.h"
#include "hls_quant.h"

static void add_golden(
    const qact_t input_a[STEM_OUT_SIZE],
    const qact_t input_b[STEM_OUT_SIZE],
    qact_t output[STEM_OUT_SIZE],

    qmult_t multiplier_a,
    qshift_t shift_a,
    qzp_t Za,

    qmult_t multiplier_b,
    qshift_t shift_b,
    qzp_t Zb,

    qzp_t Zy
) {
    for (int i = 0; i < STEM_OUT_SIZE; i++) {
        qacc_t a_centered = (qacc_t)input_a[i] - Za;
        qacc_t b_centered = (qacc_t)input_b[i] - Zb;

        qacc_t a_scaled = requantize_to_int32(
            a_centered,
            multiplier_a,
            shift_a
        );

        qacc_t b_scaled = requantize_to_int32(
            b_centered,
            multiplier_b,
            shift_b
        );

        qacc_t y = a_scaled + b_scaled + Zy;

        output[i] = clamp_u8(y);
    }
}

int main() {
    static qact_t input_a[STEM_OUT_SIZE];
    static qact_t input_b[STEM_OUT_SIZE];
    static qact_t expected[STEM_OUT_SIZE];
    static qact_t actual[STEM_OUT_SIZE];

    qzp_t Za = 127;
    qzp_t Zb = 128;
    qzp_t Zy = 126;

    qmult_t multiplier_a = 1073741824; // 0.5 Q31
    qshift_t shift_a = 0;

    qmult_t multiplier_b = 1073741824; // 0.5 Q31
    qshift_t shift_b = 0;

    printf("========================================\n");
    printf("Running ADD HLS test\n");
    printf("SIZE = %d\n", STEM_OUT_SIZE);
    printf("========================================\n");

    for (int i = 0; i < STEM_OUT_SIZE; i++) {
        input_a[i] = (qact_t)((i * 7 + 11) % 256);
        input_b[i] = (qact_t)((i * 5 + 29) % 256);
    }

    add_golden(
        input_a,
        input_b,
        expected,
        multiplier_a,
        shift_a,
        Za,
        multiplier_b,
        shift_b,
        Zb,
        Zy
    );

    add_hls(
        input_a,
        input_b,
        actual,
        multiplier_a,
        shift_a,
        Za,
        multiplier_b,
        shift_b,
        Zb,
        Zy
    );

    int max_diff = 0;
    int mismatch = 0;

    for (int i = 0; i < STEM_OUT_SIZE; i++) {
        int diff = (int)actual[i] - (int)expected[i];
        if (diff < 0) diff = -diff;

        if (diff > max_diff)
            max_diff = diff;

        if (diff != 0) {
            mismatch++;
            if (mismatch < 20) {
                printf(
                    "[Mismatch] idx=%d a=%u b=%u expected=%u actual=%u diff=%d\n",
                    i,
                    input_a[i],
                    input_b[i],
                    expected[i],
                    actual[i],
                    diff
                );
            }
        }
    }

    printf("ADD HLS test: max_diff=%d mismatch=%d\n", max_diff, mismatch);

    if (mismatch == 0) {
        printf("PASS\n");
        return 0;
    } else {
        printf("FAIL\n");
        return 1;
    }
}