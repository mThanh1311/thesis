#ifndef RESBLOCK2_HLS_H
#define RESBLOCK2_HLS_H

#include "hls_types.h"
#include "hls_config.h"

void resblock2_hls(
    const qact_t input[RES2_IN_SIZE],

    const qwgt_t weight1[RES2_W_SIZE],
    const qbias_t bias1[RES2_B_SIZE],
    const qmult_t mult1[RES2_OUT_CH],
    const qshift_t shift1[RES2_OUT_CH],
    const qzp_t weight_zp1[RES2_OUT_CH],

    const qwgt_t weight2[RES2_W_SIZE],
    const qbias_t bias2[RES2_B_SIZE],
    const qmult_t mult2[RES2_OUT_CH],
    const qshift_t shift2[RES2_OUT_CH],
    const qzp_t weight_zp2[RES2_OUT_CH],

    qact_t output[RES2_OUT_SIZE],

    qmult_t add_mult_a,
    qshift_t add_shift_a,
    qzp_t add_za,

    qmult_t add_mult_b,
    qshift_t add_shift_b,
    qzp_t add_zb,

    qzp_t Zx,
    qzp_t Zy
);

#endif