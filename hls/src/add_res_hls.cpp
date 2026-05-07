#include "add_res_hls.h"
#include "hls_quant.h"

void add_res_hls(
    const qact_t input_a[RES_OUT_SIZE],
    const qact_t input_b[RES_OUT_SIZE],
    qact_t output[RES_OUT_SIZE],

    qmult_t multiplier_a,
    qshift_t shift_a,
    qzp_t Za,

    qmult_t multiplier_b,
    qshift_t shift_b,
    qzp_t Zb,

    qzp_t Zy
) {
    for (int i = 0; i < RES_OUT_SIZE; i++) {
#pragma HLS PIPELINE II=1

        qacc_t a_centered = (qacc_t)input_a[i] - Za;
        qacc_t b_centered = (qacc_t)input_b[i] - Zb;

        qacc_t a_scaled = requantize_to_int32(
            a_centered,
            multiplier_a,
            shift_a
        );

        qacc_t b_scaled = requantize_to_int32(
            b_centered,
            multiplier_b,
            shift_b
        );

        qacc_t y = a_scaled + b_scaled + Zy;

        output[i] = clamp_u8(y);
    }
}