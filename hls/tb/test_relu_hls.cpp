#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "relu_hls.h"

static void relu_golden(
    const qact_t input[STEM_OUT_SIZE],
    qact_t output[STEM_OUT_SIZE],
    qzp_t Zy
) {
    for (int i = 0; i < STEM_OUT_SIZE; i++) {
        if (input[i] < (qact_t)Zy)
            output[i] = (qact_t)Zy;
        else
            output[i] = input[i];
    }
}

int main() {
    static qact_t input[STEM_OUT_SIZE];
    static qact_t expected[STEM_OUT_SIZE];
    static qact_t actual[STEM_OUT_SIZE];

    qzp_t Zy = 127;

    printf("========================================\n");
    printf("Running ReLU HLS test\n");
    printf("STEM_OUT_SIZE = %d\n", STEM_OUT_SIZE);
    printf("Zy = %d\n", Zy);
    printf("========================================\n");

    for (int i = 0; i < STEM_OUT_SIZE; i++) {
        input[i] = (qact_t)((i * 11 + 37) % 256);
    }

    relu_golden(input, expected, Zy);

    stem_relu_hls(input, actual, Zy);

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
                    "[Mismatch] idx=%d input=%u expected=%u actual=%u diff=%d\n",
                    i,
                    input[i],
                    expected[i],
                    actual[i],
                    diff
                );
            }
        }
    }

    printf("ReLU HLS test: max_diff=%d mismatch=%d\n", max_diff, mismatch);

    if (mismatch == 0) {
        printf("PASS\n");
        return 0;
    } else {
        printf("FAIL\n");
        return 1;
    }
}