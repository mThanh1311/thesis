#ifndef RELU_RES2_HLS_H
#define RELU_RES2_HLS_H

#include "hls_types.h"
#include "hls_config.h"

void res2_relu_hls(
    const qact_t input[RES2_OUT_SIZE],
    qact_t output[RES2_OUT_SIZE],
    qzp_t Zy
);

#endif