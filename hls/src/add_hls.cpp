#include "add_hls.h"
#include "hls_quant.h"

void add_hls(
    const qact_t input_a[STEM_OUT_SIZE],
    const qact_t input_b[STEM_OUT_SIZE],
    qact_t output[STEM_OUT_SIZE],

    qmult_t multiplier_a,
    qshift_t shift_a,
    qzp_t Za,

    qmult_t multiplier_b,
    qshift_t shift_b,
    qzp_t Zb,

    qzp_t Zy
) {
#pragma HLS INTERFACE m_axi port=input_a offset=slave bundle=gmem0
#pragma HLS INTERFACE m_axi port=input_b offset=slave bundle=gmem1
#pragma HLS INTERFACE m_axi port=output  offset=slave bundle=gmem0

#pragma HLS INTERFACE s_axilite port=input_a      bundle=control
#pragma HLS INTERFACE s_axilite port=input_b      bundle=control
#pragma HLS INTERFACE s_axilite port=output       bundle=control
#pragma HLS INTERFACE s_axilite port=multiplier_a bundle=control
#pragma HLS INTERFACE s_axilite port=shift_a      bundle=control
#pragma HLS INTERFACE s_axilite port=Za           bundle=control
#pragma HLS INTERFACE s_axilite port=multiplier_b bundle=control
#pragma HLS INTERFACE s_axilite port=shift_b      bundle=control
#pragma HLS INTERFACE s_axilite port=Zb           bundle=control
#pragma HLS INTERFACE s_axilite port=Zy           bundle=control
#pragma HLS INTERFACE s_axilite port=return       bundle=control

    for (int i = 0; i < STEM_OUT_SIZE; i++) {
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