#ifndef CONV1D_RES_HLS_H
#define CONV1D_RES_HLS_H

#include "hls_types.h"
#include "hls_config.h"

void res_conv1d_hls(
    const qact_t input[RES_IN_SIZE],
    const qwgt_t weight[RES_W_SIZE],
    const qbias_t bias[RES_B_SIZE],
    qact_t output[RES_OUT_SIZE],

    const qmult_t multiplier[RES_OUT_CH],
    const qshift_t shift[RES_OUT_CH],
    const qzp_t weight_zp[RES_OUT_CH],

    qzp_t Zx,
    qzp_t Zy
);

#endif