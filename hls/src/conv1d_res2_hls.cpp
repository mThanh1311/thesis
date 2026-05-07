#include "conv1d_res2_hls.h"
#include "hls_quant.h"

void res2_conv1d_hls(
    const qact_t input[RES2_IN_SIZE],
    const qwgt_t weight[RES2_W_SIZE],
    const qbias_t bias[RES2_B_SIZE],
    qact_t output[RES2_OUT_SIZE],

    const qmult_t multiplier[RES2_OUT_CH],
    const qshift_t shift[RES2_OUT_CH],
    const qzp_t weight_zp[RES2_OUT_CH],

    qzp_t Zx,
    qzp_t Zy
) {
    for (int oc = 0; oc < RES2_OUT_CH; oc++) {
        for (int ol = 0; ol < RES2_OUT_LEN; ol++) {
#pragma HLS PIPELINE II=1

            qacc_t acc = bias[oc];

            for (int ic = 0; ic < RES2_IN_CH; ic++) {
                for (int k = 0; k < RES2_KERNEL; k++) {

                    int il = ol * RES2_STRIDE + k - RES2_PADDING;

                    qact_t xq;

                    if (il < 0 || il >= RES2_IN_LEN)
                        xq = (qact_t)Zx;
                    else
                        xq = input[ic * RES2_IN_LEN + il];

                    qacc_t x_centered = (qacc_t)xq - Zx;

                    qwgt_t wq = weight[
                        k * RES2_IN_CH * RES2_OUT_CH +
                        ic * RES2_OUT_CH +
                        oc
                    ];

                    qacc_t w_centered =
                        (qacc_t)wq - weight_zp[oc];

                    acc += x_centered * w_centered;
                }
            }

            output[oc * RES2_OUT_LEN + ol] =
                requantize_to_u8(
                    acc,
                    multiplier[oc],
                    shift[oc],
                    Zy
                );
        }
    }
}