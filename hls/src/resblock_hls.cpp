#include "resblock_hls.h"

#include "conv1d_res_hls.h"
#include "relu_res_hls.h"
#include "add_res_hls.h"

void resblock_hls(
    const qact_t input[RES_IN_SIZE],

    const qwgt_t weight1[RES_W_SIZE],
    const qbias_t bias1[RES_B_SIZE],
    const qmult_t mult1[RES_OUT_CH],
    const qshift_t shift1[RES_OUT_CH],
    const qzp_t weight_zp1[RES_OUT_CH],

    const qwgt_t weight2[RES_W_SIZE],
    const qbias_t bias2[RES_B_SIZE],
    const qmult_t mult2[RES_OUT_CH],
    const qshift_t shift2[RES_OUT_CH],
    const qzp_t weight_zp2[RES_OUT_CH],

    qact_t output[RES_OUT_SIZE],

    qmult_t add_mult_a,
    qshift_t add_shift_a,
    qzp_t add_za,

    qmult_t add_mult_b,
    qshift_t add_shift_b,
    qzp_t add_zb,

    qzp_t Zx,
    qzp_t Zy
) {
#pragma HLS INTERFACE m_axi port=input      offset=slave bundle=gmem0
#pragma HLS INTERFACE m_axi port=weight1    offset=slave bundle=gmem1
#pragma HLS INTERFACE m_axi port=bias1      offset=slave bundle=gmem2
#pragma HLS INTERFACE m_axi port=weight2    offset=slave bundle=gmem3
#pragma HLS INTERFACE m_axi port=bias2      offset=slave bundle=gmem4
#pragma HLS INTERFACE m_axi port=output     offset=slave bundle=gmem0

#pragma HLS INTERFACE m_axi port=mult1      offset=slave bundle=gmem5
#pragma HLS INTERFACE m_axi port=shift1     offset=slave bundle=gmem5
#pragma HLS INTERFACE m_axi port=weight_zp1 offset=slave bundle=gmem5
#pragma HLS INTERFACE m_axi port=mult2      offset=slave bundle=gmem6
#pragma HLS INTERFACE m_axi port=shift2     offset=slave bundle=gmem6
#pragma HLS INTERFACE m_axi port=weight_zp2 offset=slave bundle=gmem6

#pragma HLS INTERFACE s_axilite port=return bundle=control

    static qact_t conv1_out[RES_OUT_SIZE];
    static qact_t relu1_out[RES_OUT_SIZE];
    static qact_t conv2_out[RES_OUT_SIZE];
    static qact_t add_out[RES_OUT_SIZE];

#pragma HLS BIND_STORAGE variable=conv1_out type=ram_1p impl=bram
#pragma HLS BIND_STORAGE variable=relu1_out type=ram_1p impl=bram
#pragma HLS BIND_STORAGE variable=conv2_out type=ram_1p impl=bram
#pragma HLS BIND_STORAGE variable=add_out   type=ram_1p impl=bram

    res_conv1d_hls(
        input,
        weight1,
        bias1,
        conv1_out,
        mult1,
        shift1,
        weight_zp1,
        Zx,
        Zy
    );

    res_relu_hls(
        conv1_out,
        relu1_out,
        Zy
    );

    res_conv1d_hls(
        relu1_out,
        weight2,
        bias2,
        conv2_out,
        mult2,
        shift2,
        weight_zp2,
        Zx,
        Zy
    );

    add_res_hls(
        conv2_out,
        input,
        add_out,
        add_mult_a,
        add_shift_a,
        add_za,
        add_mult_b,
        add_shift_b,
        add_zb,
        Zy
    );

    res_relu_hls(
        add_out,
        output,
        Zy
    );
}