#include "stem_hls.h"

#include "conv1d_hls.h"
#include "relu_hls.h"

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
) {

#pragma HLS INTERFACE m_axi port=input      offset=slave bundle=gmem0
#pragma HLS INTERFACE m_axi port=weight     offset=slave bundle=gmem1
#pragma HLS INTERFACE m_axi port=bias       offset=slave bundle=gmem2
#pragma HLS INTERFACE m_axi port=output     offset=slave bundle=gmem0

#pragma HLS INTERFACE m_axi port=multiplier offset=slave bundle=gmem3
#pragma HLS INTERFACE m_axi port=shift      offset=slave bundle=gmem3
#pragma HLS INTERFACE m_axi port=weight_zp  offset=slave bundle=gmem3

#pragma HLS INTERFACE s_axilite port=return bundle=control

    static qact_t conv_out[STEM_OUT_SIZE];

#pragma HLS BIND_STORAGE variable=conv_out type=ram_1p impl=bram

    stem_conv1d_hls(
        input,
        weight,
        bias,
        conv_out,
        multiplier,
        shift,
        weight_zp,
        Zx,
        Zy
    );

    stem_relu_hls(
        conv_out,
        output,
        Zy
    );
}