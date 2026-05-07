#include "relu_hls.h"

void stem_relu_hls(
    const qact_t input[STEM_OUT_SIZE],
    qact_t output[STEM_OUT_SIZE],
    qzp_t Zy
) {
#pragma HLS INTERFACE m_axi port=input  offset=slave bundle=gmem0
#pragma HLS INTERFACE m_axi port=output offset=slave bundle=gmem1

#pragma HLS INTERFACE s_axilite port=input  bundle=control
#pragma HLS INTERFACE s_axilite port=output bundle=control
#pragma HLS INTERFACE s_axilite port=Zy     bundle=control
#pragma HLS INTERFACE s_axilite port=return bundle=control

    for (int i = 0; i < STEM_OUT_SIZE; i++) {
#pragma HLS PIPELINE II=1
        qact_t x = input[i];

        if (x < (qact_t)Zy)
            output[i] = (qact_t)Zy;
        else
            output[i] = x;
    }
}