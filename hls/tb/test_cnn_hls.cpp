#include <stdio.h>
#include <stdint.h>

#include "cnn_hls.h"

int main() {
    static qact_t input[STEM_IN_SIZE];

    static qwgt_t stem_w[STEM_W_SIZE];
    static qbias_t stem_b[STEM_B_SIZE];
    static qmult_t stem_m[STEM_OUT_CH];
    static qshift_t stem_s[STEM_OUT_CH];
    static qzp_t stem_wzp[STEM_OUT_CH];

    static qwgt_t res1_w1[RES_W_SIZE];
    static qbias_t res1_b1[RES_B_SIZE];
    static qmult_t res1_m1[RES_OUT_CH];
    static qshift_t res1_s1[RES_OUT_CH];
    static qzp_t res1_wzp1[RES_OUT_CH];

    static qwgt_t res1_w2[RES_W_SIZE];
    static qbias_t res1_b2[RES_B_SIZE];
    static qmult_t res1_m2[RES_OUT_CH];
    static qshift_t res1_s2[RES_OUT_CH];
    static qzp_t res1_wzp2[RES_OUT_CH];

    static qwgt_t down_w[DOWN_W_SIZE];
    static qbias_t down_b[DOWN_B_SIZE];
    static qmult_t down_m[DOWN_OUT_CH];
    static qshift_t down_s[DOWN_OUT_CH];
    static qzp_t down_wzp[DOWN_OUT_CH];

    static qwgt_t res2_w1[RES2_W_SIZE];
    static qbias_t res2_b1[RES2_B_SIZE];
    static qmult_t res2_m1[RES2_OUT_CH];
    static qshift_t res2_s1[RES2_OUT_CH];
    static qzp_t res2_wzp1[RES2_OUT_CH];

    static qwgt_t res2_w2[RES2_W_SIZE];
    static qbias_t res2_b2[RES2_B_SIZE];
    static qmult_t res2_m2[RES2_OUT_CH];
    static qshift_t res2_s2[RES2_OUT_CH];
    static qzp_t res2_wzp2[RES2_OUT_CH];

    static qact_t output[RES2_OUT_SIZE];

    qzp_t Zx = 128;
    qzp_t Zy = 127;

    printf("========================================\n");
    printf("Running CNN HLS synthetic test\n");
    printf("CNN input size  = %d\n", STEM_IN_SIZE);
    printf("CNN output size = %d\n", RES2_OUT_SIZE);
    printf("========================================\n");

    for (int i = 0; i < STEM_IN_SIZE; i++)
        input[i] = (qact_t)((i * 7 + 13) % 256);

    for (int i = 0; i < STEM_W_SIZE; i++)
        stem_w[i] = (qwgt_t)((i * 5) % 17 - 8);

    for (int i = 0; i < RES_W_SIZE; i++) {
        res1_w1[i] = (qwgt_t)((i * 3) % 19 - 9);
        res1_w2[i] = (qwgt_t)((i * 7) % 23 - 11);
    }

    for (int i = 0; i < DOWN_W_SIZE; i++)
        down_w[i] = (qwgt_t)((i * 5) % 17 - 8);

    for (int i = 0; i < RES2_W_SIZE; i++) {
        res2_w1[i] = (qwgt_t)((i * 3) % 19 - 9);
        res2_w2[i] = (qwgt_t)((i * 7) % 23 - 11);
    }

    for (int oc = 0; oc < STEM_OUT_CH; oc++) {
        stem_b[oc] = oc - 16;
        stem_m[oc] = 1073741824;
        stem_s[oc] = 0;
        stem_wzp[oc] = 0;
    }

    for (int oc = 0; oc < RES_OUT_CH; oc++) {
        res1_b1[oc] = oc - 16;
        res1_b2[oc] = 16 - oc;

        res1_m1[oc] = 1073741824;
        res1_m2[oc] = 1073741824;

        res1_s1[oc] = 0;
        res1_s2[oc] = 0;

        res1_wzp1[oc] = 0;
        res1_wzp2[oc] = 0;
    }

    for (int oc = 0; oc < DOWN_OUT_CH; oc++) {
        down_b[oc] = oc - 32;
        down_m[oc] = 1073741824;
        down_s[oc] = 0;
        down_wzp[oc] = 0;
    }

    for (int oc = 0; oc < RES2_OUT_CH; oc++) {
        res2_b1[oc] = oc - 32;
        res2_b2[oc] = 32 - oc;

        res2_m1[oc] = 1073741824;
        res2_m2[oc] = 1073741824;

        res2_s1[oc] = 0;
        res2_s2[oc] = 0;

        res2_wzp1[oc] = 0;
        res2_wzp2[oc] = 0;
    }

    cnn_hls(
        input,
        stem_w,
        stem_b,
        stem_m,
        stem_s,
        stem_wzp,
        res1_w1,
        res1_b1,
        res1_m1,
        res1_s1,
        res1_wzp1,
        res1_w2,
        res1_b2,
        res1_m2,
        res1_s2,
        res1_wzp2,
        down_w,
        down_b,
        down_m,
        down_s,
        down_wzp,
        res2_w1,
        res2_b1,
        res2_m1,
        res2_s1,
        res2_wzp1,
        res2_w2,
        res2_b2,
        res2_m2,
        res2_s2,
        res2_wzp2,
        output,
        Zx,
        Zy
    );

    printf("CNN HLS synthetic PASS\n");
    return 0;
}