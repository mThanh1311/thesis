#ifndef RELU_RES_HLS_H
#define RELU_RES_HLS_H

#include "hls_types.h"
#include "hls_config.h"

void res_relu_hls(
    const qact_t input[RES_OUT_SIZE],
    qact_t output[RES_OUT_SIZE],
    qzp_t Zy
);

#endif