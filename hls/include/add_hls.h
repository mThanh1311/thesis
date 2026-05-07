#ifndef ADD_HLS_H
#define ADD_HLS_H

#include "hls_types.h"
#include "hls_config.h"

void add_hls(
    const qact_t input_a[STEM_OUT_SIZE],
    const qact_t input_b[STEM_OUT_SIZE],
    qact_t output[STEM_OUT_SIZE],

    qmult_t multiplier_a,
    qshift_t shift_a,
    qzp_t Za,

    qmult_t multiplier_b,
    qshift_t shift_b,
    qzp_t Zb,

    qzp_t Zy
);

#endif