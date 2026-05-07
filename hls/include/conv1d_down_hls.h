#ifndef CONV1D_DOWN_HLS_H
#define CONV1D_DOWN_HLS_H

#include "hls_types.h"
#include "hls_config.h"

void down_conv1d_hls(
    const qact_t input[DOWN_IN_SIZE],
    const qwgt_t weight[DOWN_W_SIZE],
    const qbias_t bias[DOWN_B_SIZE],
    qact_t output[DOWN_OUT_SIZE],

    const qmult_t multiplier[DOWN_OUT_CH],
    const qshift_t shift[DOWN_OUT_CH],
    const qzp_t weight_zp[DOWN_OUT_CH],

    qzp_t Zx,
    qzp_t Zy
);

#endif