#ifndef ADD_RES2_HLS_H
#define ADD_RES2_HLS_H

#include "hls_types.h"
#include "hls_config.h"

void add_res2_hls(
    const qact_t input_a[RES2_OUT_SIZE],
    const qact_t input_b[RES2_OUT_SIZE],
    qact_t output[RES2_OUT_SIZE],

    qmult_t multiplier_a,
    qshift_t shift_a,
    qzp_t Za,

    qmult_t multiplier_b,
    qshift_t shift_b,
    qzp_t Zb,

    qzp_t Zy
);

#endif