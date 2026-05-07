#include "cnn_hls.h"

#include "stem_hls.h"
#include "resblock_hls.h"
#include "downsample_hls.h"
#include "resblock2_hls.h"

void cnn_hls(
    const qact_t input[STEM_IN_SIZE],

    const qwgt_t stem_w[STEM_W_SIZE],
    const qbias_t stem_b[STEM_B_SIZE],
    const qmult_t stem_m[STEM_OUT_CH],
    const qshift_t stem_s[STEM_OUT_CH],
    const qzp_t stem_wzp[STEM_OUT_CH],

    const qwgt_t res1_w1[RES_W_SIZE],
    const qbias_t res1_b1[RES_B_SIZE],
    const qmult_t res1_m1[RES_OUT_CH],
    const qshift_t res1_s1[RES_OUT_CH],
    const qzp_t res1_wzp1[RES_OUT_CH],

    const qwgt_t res1_w2[RES_W_SIZE],
    const qbias_t res1_b2[RES_B_SIZE],
    const qmult_t res1_m2[RES_OUT_CH],
    const qshift_t res1_s2[RES_OUT_CH],
    const qzp_t res1_wzp2[RES_OUT_CH],

    const qwgt_t down_w[DOWN_W_SIZE],
    const qbias_t down_b[DOWN_B_SIZE],
    const qmult_t down_m[DOWN_OUT_CH],
    const qshift_t down_s[DOWN_OUT_CH],
    const qzp_t down_wzp[DOWN_OUT_CH],

    const qwgt_t res2_w1[RES2_W_SIZE],
    const qbias_t res2_b1[RES2_B_SIZE],
    const qmult_t res2_m1[RES2_OUT_CH],
    const qshift_t res2_s1[RES2_OUT_CH],
    const qzp_t res2_wzp1[RES2_OUT_CH],

    const qwgt_t res2_w2[RES2_W_SIZE],
    const qbias_t res2_b2[RES2_B_SIZE],
    const qmult_t res2_m2[RES2_OUT_CH],
    const qshift_t res2_s2[RES2_OUT_CH],
    const qzp_t res2_wzp2[RES2_OUT_CH],

    qact_t output[RES2_OUT_SIZE],

    qzp_t Zx,
    qzp_t Zy
) {
    static qact_t stem_out[STEM_OUT_SIZE];
    static qact_t res1_out[RES_OUT_SIZE];
    static qact_t down_out[DOWN_OUT_SIZE];

#pragma HLS BIND_STORAGE variable=stem_out type=ram_1p impl=bram
#pragma HLS BIND_STORAGE variable=res1_out type=ram_1p impl=bram
#pragma HLS BIND_STORAGE variable=down_out type=ram_1p impl=bram

    stem_hls(
        input,
        stem_w,
        stem_b,
        stem_out,
        stem_m,
        stem_s,
        stem_wzp,
        Zx,
        Zy
    );

    resblock_hls(
        stem_out,
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
        res1_out,
        1073741824,
        0,
        Zy,
        1073741824,
        0,
        Zy,
        Zy,
        Zy
    );

    downsample_hls(
        res1_out,
        down_w,
        down_b,
        down_m,
        down_s,
        down_wzp,
        down_out,
        Zy,
        Zy
    );

    resblock2_hls(
        down_out,
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
        1073741824,
        0,
        Zy,
        1073741824,
        0,
        Zy,
        Zy,
        Zy
    );
}