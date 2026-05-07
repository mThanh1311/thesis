#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "stem_hls.h"

int main() {

    static qact_t input[STEM_IN_SIZE];
    static qwgt_t weight[STEM_W_SIZE];
    static qbias_t bias[STEM_B_SIZE];

    static qact_t output[STEM_OUT_SIZE];

    static qmult_t multiplier[STEM_OUT_CH];
    static qshift_t shift[STEM_OUT_CH];
    static qzp_t weight_zp[STEM_OUT_CH];

    qzp_t Zx = 128;
    qzp_t Zy = 127;

    printf("========================================\n");
    printf("Running STEM HLS test\n");
    printf("========================================\n");

    for (int i = 0; i < STEM_IN_SIZE; i++) {
        input[i] = (qact_t)((i * 7 + 13) % 256);
    }

    for (int i = 0; i < STEM_W_SIZE; i++) {
        weight[i] = (qwgt_t)((i * 5) % 17 - 8);
    }

    for (int oc = 0; oc < STEM_OUT_CH; oc++) {
        bias[oc] = oc - 16;

        multiplier[oc] = 1073741824;
        shift[oc] = 0;
        weight_zp[oc] = 0;
    }

    stem_hls(
        input,
        weight,
        bias,
        output,
        multiplier,
        shift,
        weight_zp,
        Zx,
        Zy
    );

    printf("STEM HLS PASS\n");

    return 0;
}