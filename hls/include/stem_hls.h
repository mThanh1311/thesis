#ifndef STEM_HLS_H
#define STEM_HLS_H

#include "hls_types.h"
#include "hls_config.h"

void stem_hls(
    const qact_t input[STEM_IN_SIZE],
    const qwgt_t weight[STEM_W_SIZE],
    const qbias_t bias[STEM_B_SIZE],

    qact_t output[STEM_OUT_SIZE],

    const qmult_t multiplier[STEM_OUT_CH],
    const qshift_t shift[STEM_OUT_CH],
    const qzp_t weight_zp[STEM_OUT_CH],

    qzp_t Zx,
    qzp_t Zy
);

#endif