#include "conv1d_down_hls.h"
#include "hls_quant.h"

void down_conv1d_hls(
    const qact_t input[DOWN_IN_SIZE],
    const qwgt_t weight[DOWN_W_SIZE],
    const qbias_t bias[DOWN_B_SIZE],
    qact_t output[DOWN_OUT_SIZE],

    const qmult_t multiplier[DOWN_OUT_CH],
    const qshift_t shift[DOWN_OUT_CH],
    const qzp_t weight_zp[DOWN_OUT_CH],

    qzp_t Zx,
    qzp_t Zy
) {
    for (int oc = 0; oc < DOWN_OUT_CH; oc++) {
        for (int ol = 0; ol < DOWN_OUT_LEN; ol++) {
#pragma HLS PIPELINE II=1

            qacc_t acc = bias[oc];

            for (int ic = 0; ic < DOWN_IN_CH; ic++) {
                for (int k = 0; k < DOWN_KERNEL; k++) {

                    int il = ol * DOWN_STRIDE + k - DOWN_PADDING;

                    qact_t xq;

                    if (il < 0 || il >= DOWN_IN_LEN)
                        xq = (qact_t)Zx;
                    else
                        xq = input[ic * DOWN_IN_LEN + il];

                    qacc_t x_centered = (qacc_t)xq - Zx;

                    qwgt_t wq = weight[
                        k * DOWN_IN_CH * DOWN_OUT_CH +
                        ic * DOWN_OUT_CH +
                        oc
                    ];

                    qacc_t w_centered =
                        (qacc_t)wq - weight_zp[oc];

                    acc += x_centered * w_centered;
                }
            }

            output[oc * DOWN_OUT_LEN + ol] =
                requantize_to_u8(
                    acc,
                    multiplier[oc],
                    shift[oc],
                    Zy
                );
        }
    }
}