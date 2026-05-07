#include "relu_res_hls.h"

void res_relu_hls(
    const qact_t input[RES_OUT_SIZE],
    qact_t output[RES_OUT_SIZE],
    qzp_t Zy
) {
    for (int i = 0; i < RES_OUT_SIZE; i++) {
#pragma HLS PIPELINE II=1
        qact_t x = input[i];

        if (x < (qact_t)Zy)
            output[i] = (qact_t)Zy;
        else
            output[i] = x;
    }
}