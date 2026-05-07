#ifndef RELU_DOWN_HLS_H
#define RELU_DOWN_HLS_H

#include "hls_types.h"
#include "hls_config.h"

void down_relu_hls(
    const qact_t input[DOWN_OUT_SIZE],
    qact_t output[DOWN_OUT_SIZE],
    qzp_t Zy
);

#endif