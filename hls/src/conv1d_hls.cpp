#include "conv1d_hls.h"
#include "hls_quant.h"

void stem_conv1d_hls(
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

#pragma HLS INTERFACE s_axilite port=input      bundle=control
#pragma HLS INTERFACE s_axilite port=weight     bundle=control
#pragma HLS INTERFACE s_axilite port=bias       bundle=control
#pragma HLS INTERFACE s_axilite port=output     bundle=control
#pragma HLS INTERFACE s_axilite port=multiplier bundle=control
#pragma HLS INTERFACE s_axilite port=shift      bundle=control
#pragma HLS INTERFACE s_axilite port=weight_zp  bundle=control
#pragma HLS INTERFACE s_axilite port=Zx         bundle=control
#pragma HLS INTERFACE s_axilite port=Zy         bundle=control
#pragma HLS INTERFACE s_axilite port=return     bundle=control

    for (int oc = 0; oc < STEM_OUT_CH; oc++) {
        for (int ol = 0; ol < STEM_OUT_LEN; ol++) {
#pragma HLS PIPELINE II=1

            qacc_t acc = bias[oc];

            for (int ic = 0; ic < STEM_IN_CH; ic++) {
                for (int k = 0; k < STEM_KERNEL; k++) {

                    int il = ol * STEM_STRIDE + k - STEM_PADDING;

                    qact_t xq;

                    if (il < 0 || il >= STEM_IN_LEN) {
                        xq = (qact_t)Zx;
                    } else {
                        xq = input[ic * STEM_IN_LEN + il];
                    }

                    qacc_t x_centered = (qacc_t)xq - Zx;

                    // weight layout:
                    // weight[k * in_ch * out_ch + ic * out_ch + oc]
                    qwgt_t wq = weight[
                        k * STEM_IN_CH * STEM_OUT_CH +
                        ic * STEM_OUT_CH +
                        oc
                    ];

                    qacc_t w_centered = (qacc_t)wq - weight_zp[oc];

                    acc += x_centered * w_centered;
                }
            }

            output[oc * STEM_OUT_LEN + ol] =
                requantize_to_u8(
                    acc,
                    multiplier[oc],
                    shift[oc],
                    Zy
                );
        }
    }
}