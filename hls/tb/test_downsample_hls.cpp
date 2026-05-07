#include <stdio.h>
#include <stdint.h>

#include "downsample_hls.h"

int main() {

    static qact_t input[DOWN_IN_SIZE];

    static qwgt_t weight[DOWN_W_SIZE];
    static qbias_t bias[DOWN_B_SIZE];

    static qmult_t multiplier[DOWN_OUT_CH];
    static qshift_t shift[DOWN_OUT_CH];
    static qzp_t weight_zp[DOWN_OUT_CH];

    static qact_t output[DOWN_OUT_SIZE];

    qzp_t Zx = 128;
    qzp_t Zy = 127;

    printf("========================================\n");
    printf("Running DOWNSAMPLE HLS synthetic test\n");
    printf("DOWN_IN_SIZE  = %d\n", DOWN_IN_SIZE);
    printf("DOWN_OUT_SIZE = %d\n", DOWN_OUT_SIZE);
    printf("DOWN_W_SIZE   = %d\n", DOWN_W_SIZE);
    printf("========================================\n");

    for (int i = 0; i < DOWN_IN_SIZE; i++) {
        input[i] = (qact_t)((i * 7 + 13) % 256);
    }

    for (int i = 0; i < DOWN_W_SIZE; i++) {
        weight[i] = (qwgt_t)((i * 5) % 17 - 8);
    }

    for (int oc = 0; oc < DOWN_OUT_CH; oc++) {

        bias[oc] = oc - 32;

        multiplier[oc] = 1073741824;
        shift[oc] = 0;

        weight_zp[oc] = 0;
    }

    downsample_hls(
        input,
        weight,
        bias,
        multiplier,
        shift,
        weight_zp,
        output,
        Zx,
        Zy
    );

    printf("DOWNSAMPLE HLS synthetic PASS\n");

    return 0;
}