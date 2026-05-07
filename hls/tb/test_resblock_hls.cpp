#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "resblock_hls.h"

int main() {
    static qact_t input[RES_IN_SIZE];

    static qwgt_t weight1[RES_W_SIZE];
    static qbias_t bias1[RES_B_SIZE];
    static qmult_t mult1[RES_OUT_CH];
    static qshift_t shift1[RES_OUT_CH];
    static qzp_t weight_zp1[RES_OUT_CH];

    static qwgt_t weight2[RES_W_SIZE];
    static qbias_t bias2[RES_B_SIZE];
    static qmult_t mult2[RES_OUT_CH];
    static qshift_t shift2[RES_OUT_CH];
    static qzp_t weight_zp2[RES_OUT_CH];

    static qact_t output[RES_OUT_SIZE];

    qzp_t Zx = 128;
    qzp_t Zy = 127;

    qmult_t add_mult_a = 1073741824;
    qshift_t add_shift_a = 0;
    qzp_t add_za = Zy;

    qmult_t add_mult_b = 1073741824;
    qshift_t add_shift_b = 0;
    qzp_t add_zb = Zx;

    printf("========================================\n");
    printf("Running RESBLOCK HLS synthetic test\n");
    printf("RES_IN_SIZE  = %d\n", RES_IN_SIZE);
    printf("RES_OUT_SIZE = %d\n", RES_OUT_SIZE);
    printf("RES_W_SIZE   = %d\n", RES_W_SIZE);
    printf("========================================\n");

    for (int i = 0; i < RES_IN_SIZE; i++) {
        input[i] = (qact_t)((i * 7 + 31) % 256);
    }

    for (int i = 0; i < RES_W_SIZE; i++) {
        weight1[i] = (qwgt_t)((i * 5) % 17 - 8);
        weight2[i] = (qwgt_t)((i * 3) % 19 - 9);
    }

    for (int oc = 0; oc < RES_OUT_CH; oc++) {
        bias1[oc] = oc - 16;
        bias2[oc] = 16 - oc;

        mult1[oc] = 1073741824;
        mult2[oc] = 1073741824;

        shift1[oc] = 0;
        shift2[oc] = 0;

        weight_zp1[oc] = 0;
        weight_zp2[oc] = 0;
    }

    resblock_hls(
        input,
        weight1,
        bias1,
        mult1,
        shift1,
        weight_zp1,
        weight2,
        bias2,
        mult2,
        shift2,
        weight_zp2,
        output,
        add_mult_a,
        add_shift_a,
        add_za,
        add_mult_b,
        add_shift_b,
        add_zb,
        Zx,
        Zy
    );

    printf("RESBLOCK HLS synthetic PASS\n");

    return 0;
}