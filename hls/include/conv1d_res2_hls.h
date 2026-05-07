#ifndef CONV1D_RES2_HLS_H
#define CONV1D_RES2_HLS_H

#include "hls_types.h"
#include "hls_config.h"

void res2_conv1d_hls(
    const qact_t input[RES2_IN_SIZE],
    const qwgt_t weight[RES2_W_SIZE],
    const qbias_t bias[RES2_B_SIZE],
    qact_t output[RES2_OUT_SIZE],

    const qmult_t multiplier[RES2_OUT_CH],
    const qshift_t shift[RES2_OUT_CH],
    const qzp_t weight_zp[RES2_OUT_CH],

    qzp_t Zx,
    qzp_t Zy
);

#endif