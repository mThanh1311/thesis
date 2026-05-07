#ifndef CNN_REALDATA_HLS_H
#define CNN_REALDATA_HLS_H

#include "hls_types.h"
#include "hls_config.h"

void cnn_realdata_hls(
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

    qmult_t res1_add_ma,
    qshift_t res1_add_sa,
    qzp_t res1_za,
    qmult_t res1_add_mb,
    qshift_t res1_add_sb,
    qzp_t res1_zb,
    qzp_t res1_zy,

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

    qmult_t res2_add_ma,
    qshift_t res2_add_sa,
    qzp_t res2_za,
    qmult_t res2_add_mb,
    qshift_t res2_add_sb,
    qzp_t res2_zb,
    qzp_t res2_zy,

    qact_t output[RES2_OUT_SIZE],

    qzp_t stem_zx,
    qzp_t stem_zy,
    qzp_t res1_conv1_zx,
    qzp_t res1_conv1_zy,
    qzp_t res1_conv2_zx,
    qzp_t res1_conv2_zy,
    qzp_t down_zx,
    qzp_t down_zy,
    qzp_t res2_conv1_zx,
    qzp_t res2_conv1_zy,
    qzp_t res2_conv2_zx,
    qzp_t res2_conv2_zy
);

#endif