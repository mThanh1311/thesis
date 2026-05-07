#ifndef DOWNSAMPLE_HLS_H
#define DOWNSAMPLE_HLS_H

#include "hls_types.h"
#include "hls_config.h"

void downsample_hls(
    const qact_t input[DOWN_IN_SIZE],

    const qwgt_t weight[DOWN_W_SIZE],
    const qbias_t bias[DOWN_B_SIZE],

    const qmult_t multiplier[DOWN_OUT_CH],
    const qshift_t shift[DOWN_OUT_CH],
    const qzp_t weight_zp[DOWN_OUT_CH],

    qact_t output[DOWN_OUT_SIZE],

    qzp_t Zx,
    qzp_t Zy
);

#endif