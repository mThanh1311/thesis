#include "conv1d_res_hls.h"
#include "hls_quant.h"

void res_conv1d_hls(
    const qact_t input[RES_IN_SIZE],
    const qwgt_t weight[RES_W_SIZE],
    const qbias_t bias[RES_B_SIZE],
    qact_t output[RES_OUT_SIZE],

    const qmult_t multiplier[RES_OUT_CH],
    const qshift_t shift[RES_OUT_CH],
    const qzp_t weight_zp[RES_OUT_CH],

    qzp_t Zx,
    qzp_t Zy
) {
    for (int oc = 0; oc < RES_OUT_CH; oc++) {
        for (int ol = 0; ol < RES_OUT_LEN; ol++) {
#pragma HLS PIPELINE II=1

            qacc_t acc = bias[oc];

            for (int ic = 0; ic < RES_IN_CH; ic++) {
                for (int k = 0; k < RES_KERNEL; k++) {

                    int il = ol * RES_STRIDE + k - RES_PADDING;

                    qact_t xq;

                    if (il < 0 || il >= RES_IN_LEN)
                        xq = (qact_t)Zx;
                    else
                        xq = input[ic * RES_IN_LEN + il];

                    qacc_t x_centered = (qacc_t)xq - Zx;

                    qwgt_t wq = weight[
                        k * RES_IN_CH * RES_OUT_CH +
                        ic * RES_OUT_CH +
                        oc
                    ];

                    qacc_t w_centered = (qacc_t)wq - weight_zp[oc];

                    acc += x_centered * w_centered;
                }
            }

            output[oc * RES_OUT_LEN + ol] =
                requantize_to_u8(
                    acc,
                    multiplier[oc],
                    shift[oc],
                    Zy
                );
        }
    }
}