#include "downsample_hls.h"

#include "conv1d_down_hls.h"
#include "relu_down_hls.h"

void downsample_hls(
    const qact_t input[DOWN_IN_SIZE],

    const qwgt_t weight[DOWN_W_SIZE],
    const qbias_t bias[DOWN_B_SIZE],

    const qmult_t multiplier[DOWN_OUT_CH],
    const qshift_t shift[DOWN_OUT_CH],
    const qzp_t weight_zp[DOWN_OUT_CH],

    qact_t output[DOWN_OUT_SIZE],

    qzp_t Zx,
    qzp_t Zy
) {

    static qact_t conv_out[DOWN_OUT_SIZE];

#pragma HLS BIND_STORAGE variable=conv_out type=ram_1p impl=bram

    down_conv1d_hls(
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

    down_relu_hls(
        conv_out,
        output,
        Zy
    );
}