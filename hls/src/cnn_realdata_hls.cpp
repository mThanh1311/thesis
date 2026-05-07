#include "cnn_realdata_hls.h"

#include "conv1d_hls.h"
#include "relu_hls.h"

#include "conv1d_res_hls.h"
#include "relu_res_hls.h"
#include "add_res_hls.h"

#include "conv1d_down_hls.h"
#include "relu_down_hls.h"

#include "conv1d_res2_hls.h"
#include "relu_res2_hls.h"
#include "add_res2_hls.h"

void cnn_realdata_hls(
    const qact_t input[STEM_IN_SIZE],

    const qwgt_t stem_w[STEM_W_SIZE],
    const qbias_t stem_b[STEM_B_SIZE],
    const qmult_t stem_m[STEM_OUT_CH],
    const qshift_t stem_s[STEM_OUT_CH],
    const qzp_t stem_wzp[STEM_OUT_CH],

    const qwgt_t res1_w1[RES_W_SIZE],
    const qbias_t res1_b1[RES_B_SIZE],
    const qmult_t res1_m1[RES_OUT_CH],
    const qshift_t res1_s1[RES_OUT_CH],
    const qzp_t res1_wzp1[RES_OUT_CH],

    const qwgt_t res1_w2[RES_W_SIZE],
    const qbias_t res1_b2[RES_B_SIZE],
    const qmult_t res1_m2[RES_OUT_CH],
    const qshift_t res1_s2[RES_OUT_CH],
    const qzp_t res1_wzp2[RES_OUT_CH],

    qmult_t res1_add_ma,
    qshift_t res1_add_sa,
    qzp_t res1_za,
    qmult_t res1_add_mb,
    qshift_t res1_add_sb,
    qzp_t res1_zb,
    qzp_t res1_zy,

    const qwgt_t down_w[DOWN_W_SIZE],
    const qbias_t down_b[DOWN_B_SIZE],
    const qmult_t down_m[DOWN_OUT_CH],
    const qshift_t down_s[DOWN_OUT_CH],
    const qzp_t down_wzp[DOWN_OUT_CH],

    const qwgt_t res2_w1[RES2_W_SIZE],
    const qbias_t res2_b1[RES2_B_SIZE],
    const qmult_t res2_m1[RES2_OUT_CH],
    const qshift_t res2_s1[RES2_OUT_CH],
    const qzp_t res2_wzp1[RES2_OUT_CH],

    const qwgt_t res2_w2[RES2_W_SIZE],
    const qbias_t res2_b2[RES2_B_SIZE],
    const qmult_t res2_m2[RES2_OUT_CH],
    const qshift_t res2_s2[RES2_OUT_CH],
    const qzp_t res2_wzp2[RES2_OUT_CH],

    qmult_t res2_add_ma,
    qshift_t res2_add_sa,
    qzp_t res2_za,
    qmult_t res2_add_mb,
    qshift_t res2_add_sb,
    qzp_t res2_zb,
    qzp_t res2_zy,

    qact_t output[RES2_OUT_SIZE],

    qzp_t stem_zx,
    qzp_t stem_zy,
    qzp_t res1_conv1_zx,
    qzp_t res1_conv1_zy,
    qzp_t res1_conv2_zx,
    qzp_t res1_conv2_zy,
    qzp_t down_zx,
    qzp_t down_zy,
    qzp_t res2_conv1_zx,
    qzp_t res2_conv1_zy,
    qzp_t res2_conv2_zx,
    qzp_t res2_conv2_zy
) {
    static qact_t stem_conv_out[STEM_OUT_SIZE];
    static qact_t stem_out[STEM_OUT_SIZE];

    static qact_t res1_c1[RES_OUT_SIZE];
    static qact_t res1_r1[RES_OUT_SIZE];
    static qact_t res1_c2[RES_OUT_SIZE];
    static qact_t res1_add[RES_OUT_SIZE];
    static qact_t res1_out[RES_OUT_SIZE];

    static qact_t down_conv[DOWN_OUT_SIZE];
    static qact_t down_out[DOWN_OUT_SIZE];

    static qact_t res2_c1[RES2_OUT_SIZE];
    static qact_t res2_r1[RES2_OUT_SIZE];
    static qact_t res2_c2[RES2_OUT_SIZE];
    static qact_t res2_add[RES2_OUT_SIZE];

#pragma HLS BIND_STORAGE variable=stem_conv_out type=ram_1p impl=bram
#pragma HLS BIND_STORAGE variable=stem_out      type=ram_1p impl=bram
#pragma HLS BIND_STORAGE variable=res1_c1       type=ram_1p impl=bram
#pragma HLS BIND_STORAGE variable=res1_r1       type=ram_1p impl=bram
#pragma HLS BIND_STORAGE variable=res1_c2       type=ram_1p impl=bram
#pragma HLS BIND_STORAGE variable=res1_add      type=ram_1p impl=bram
#pragma HLS BIND_STORAGE variable=res1_out      type=ram_1p impl=bram
#pragma HLS BIND_STORAGE variable=down_conv     type=ram_1p impl=bram
#pragma HLS BIND_STORAGE variable=down_out      type=ram_1p impl=bram
#pragma HLS BIND_STORAGE variable=res2_c1       type=ram_1p impl=bram
#pragma HLS BIND_STORAGE variable=res2_r1       type=ram_1p impl=bram
#pragma HLS BIND_STORAGE variable=res2_c2       type=ram_1p impl=bram
#pragma HLS BIND_STORAGE variable=res2_add      type=ram_1p impl=bram

    stem_conv1d_hls(input, stem_w, stem_b, stem_conv_out,
                    stem_m, stem_s, stem_wzp, stem_zx, stem_zy);

    stem_relu_hls(stem_conv_out, stem_out, stem_zy);

    res_conv1d_hls(stem_out, res1_w1, res1_b1, res1_c1,
                   res1_m1, res1_s1, res1_wzp1,
                   res1_conv1_zx, res1_conv1_zy);

    res_relu_hls(res1_c1, res1_r1, res1_conv1_zy);

    res_conv1d_hls(res1_r1, res1_w2, res1_b2, res1_c2,
                   res1_m2, res1_s2, res1_wzp2,
                   res1_conv2_zx, res1_conv2_zy);

    add_res_hls(res1_c2, stem_out, res1_add,
                res1_add_ma, res1_add_sa, res1_za,
                res1_add_mb, res1_add_sb, res1_zb,
                res1_zy);

    res_relu_hls(res1_add, res1_out, res1_zy);

    down_conv1d_hls(res1_out, down_w, down_b, down_conv,
                    down_m, down_s, down_wzp,
                    down_zx, down_zy);

    down_relu_hls(down_conv, down_out, down_zy);

    res2_conv1d_hls(down_out, res2_w1, res2_b1, res2_c1,
                    res2_m1, res2_s1, res2_wzp1,
                    res2_conv1_zx, res2_conv1_zy);

    res2_relu_hls(res2_c1, res2_r1, res2_conv1_zy);

    res2_conv1d_hls(res2_r1, res2_w2, res2_b2, res2_c2,
                    res2_m2, res2_s2, res2_wzp2,
                    res2_conv2_zx, res2_conv2_zy);

    add_res2_hls(res2_c2, down_out, res2_add,
                 res2_add_ma, res2_add_sa, res2_za,
                 res2_add_mb, res2_add_sb, res2_zb,
                 res2_zy);

    res2_relu_hls(res2_add, output, res2_zy);
}