#ifndef RELU_HLS_H
#define RELU_HLS_H

#include "hls_types.h"
#include "hls_config.h"

void stem_relu_hls(
    const qact_t input[STEM_OUT_SIZE],
    qact_t output[STEM_OUT_SIZE],
    qzp_t Zy
);

#endif