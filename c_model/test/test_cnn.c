#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "load_txt.h"
#include "cnn.h"

#define IN_SIZE 320

#define RES1_SIZE (32 * 160)
#define RES2_SIZE (64 * 80)

#define STEM_W_SIZE (32 * 1 * 5)
#define RES1_W_SIZE (32 * 32 * 5)
#define DOWN_W_SIZE (64 * 32 * 5)
#define RES2_W_SIZE (64 * 64 * 5)

int main() {

    const int CNN_TOL = 20;

    uint8_t *input  = malloc(IN_SIZE * sizeof(uint8_t));
    uint8_t *output = malloc(RES2_SIZE * sizeof(uint8_t));
    uint8_t *golden = malloc(RES2_SIZE * sizeof(uint8_t));

    // ===== STEM =====
    int8_t  *stem_w  = malloc(STEM_W_SIZE * sizeof(int8_t));
    int32_t *stem_b  = malloc(32 * sizeof(int32_t));
    int32_t *stem_m  = malloc(32 * sizeof(int32_t));
    int32_t *stem_s  = malloc(32 * sizeof(int32_t));
    int32_t *stem_zw = malloc(32 * sizeof(int32_t));
    int32_t stem_Zx, stem_Zy, stem_relu_Zy;

    // ===== RES1 =====
    int8_t  *res1_w1  = malloc(RES1_W_SIZE * sizeof(int8_t));
    int32_t *res1_b1  = malloc(32 * sizeof(int32_t));
    int32_t *res1_m1  = malloc(32 * sizeof(int32_t));
    int32_t *res1_s1  = malloc(32 * sizeof(int32_t));
    int32_t *res1_zw1 = malloc(32 * sizeof(int32_t));

    int8_t  *res1_w2  = malloc(RES1_W_SIZE * sizeof(int8_t));
    int32_t *res1_b2  = malloc(32 * sizeof(int32_t));
    int32_t *res1_m2  = malloc(32 * sizeof(int32_t));
    int32_t *res1_s2  = malloc(32 * sizeof(int32_t));
    int32_t *res1_zw2 = malloc(32 * sizeof(int32_t));

    int32_t res1_conv1_Zx, res1_conv1_Zy;
    int32_t res1_conv2_Zx, res1_conv2_Zy;

    float res1_S_main, res1_S_skip, res1_S_add;
    int32_t res1_Z_main, res1_Z_skip, res1_Z_add;
    int32_t res1_relu_Zy;

    // ===== DOWN1 =====
    int8_t  *down_w  = malloc(DOWN_W_SIZE * sizeof(int8_t));
    int32_t *down_b  = malloc(64 * sizeof(int32_t));
    int32_t *down_m  = malloc(64 * sizeof(int32_t));
    int32_t *down_s  = malloc(64 * sizeof(int32_t));
    int32_t *down_zw = malloc(64 * sizeof(int32_t));
    int32_t down_Zx, down_Zy, down_relu_Zy;

    // ===== RES2 =====
    int8_t  *res2_w1  = malloc(RES2_W_SIZE * sizeof(int8_t));
    int32_t *res2_b1  = malloc(64 * sizeof(int32_t));
    int32_t *res2_m1  = malloc(64 * sizeof(int32_t));
    int32_t *res2_s1  = malloc(64 * sizeof(int32_t));
    int32_t *res2_zw1 = malloc(64 * sizeof(int32_t));

    int8_t  *res2_w2  = malloc(RES2_W_SIZE * sizeof(int8_t));
    int32_t *res2_b2  = malloc(64 * sizeof(int32_t));
    int32_t *res2_m2  = malloc(64 * sizeof(int32_t));
    int32_t *res2_s2  = malloc(64 * sizeof(int32_t));
    int32_t *res2_zw2 = malloc(64 * sizeof(int32_t));

    int32_t res2_conv1_Zx, res2_conv1_Zy;
    int32_t res2_conv2_Zx, res2_conv2_Zy;

    float res2_S_main, res2_S_skip, res2_S_add;
    int32_t res2_Z_main, res2_Z_skip, res2_Z_add;
    int32_t res2_relu_Zy;

    // ===== LOAD INPUT / GOLDEN =====
    load_uint8("data/q0_input_quant_out_u8.txt", input, IN_SIZE);
    load_uint8("data/q1_cnn_dequant_in_u8.txt", golden, RES2_SIZE);

    // ===== LOAD STEM =====
    load_int8 ("data/stem.0_weight_int8.txt", stem_w, STEM_W_SIZE);
    load_int32("data/stem.0_bias_int32.txt", stem_b, 32);
    load_int32("data/stem.0_multiplier.txt", stem_m, 32);
    load_int32("data/stem.0_shift.txt", stem_s, 32);
    load_int32("data/stem.0_weight_zp.txt", stem_zw, 32);

    load_int32("data/l0_stem_conv_Zx.txt", &stem_Zx, 1);
    load_int32("data/l0_stem_conv_Zy.txt", &stem_Zy, 1);
    load_int32("data/l0b_stem_relu_Zy.txt", &stem_relu_Zy, 1);

    // ===== LOAD RES1 =====
    load_int8 ("data/res1.block.0_weight_int8.txt", res1_w1, RES1_W_SIZE);
    load_int32("data/res1.block.0_bias_int32.txt", res1_b1, 32);
    load_int32("data/res1.block.0_multiplier.txt", res1_m1, 32);
    load_int32("data/res1.block.0_shift.txt", res1_s1, 32);
    load_int32("data/res1.block.0_weight_zp.txt", res1_zw1, 32);

    load_int8 ("data/res1.block.2_weight_int8.txt", res1_w2, RES1_W_SIZE);
    load_int32("data/res1.block.2_bias_int32.txt", res1_b2, 32);
    load_int32("data/res1.block.2_multiplier.txt", res1_m2, 32);
    load_int32("data/res1.block.2_shift.txt", res1_s2, 32);
    load_int32("data/res1.block.2_weight_zp.txt", res1_zw2, 32);

    load_int32("data/l1_res1_conv1_Zx.txt", &res1_conv1_Zx, 1);
    load_int32("data/l1_res1_conv1_Zy.txt", &res1_conv1_Zy, 1);
    load_int32("data/l2_res1_conv2_Zx.txt", &res1_conv2_Zx, 1);
    load_int32("data/l2_res1_conv2_Zy.txt", &res1_conv2_Zy, 1);

    load_float("data/l2_res1_conv2_Sy.txt", &res1_S_main, 1);
    load_float("data/l1_res1_conv1_Sx.txt", &res1_S_skip, 1);
    load_float("data/l3b_res1_relu_out_Sx.txt", &res1_S_add, 1);

    load_int32("data/l2_res1_conv2_Zy.txt", &res1_Z_main, 1);
    load_int32("data/l1_res1_conv1_Zx.txt", &res1_Z_skip, 1);
    load_int32("data/l3b_res1_relu_out_Zx.txt", &res1_Z_add, 1);

    load_int32("data/l3b_res1_relu_out_Zy.txt", &res1_relu_Zy, 1);

    // ===== LOAD DOWN1 =====
    load_int8 ("data/down1.0_weight_int8.txt", down_w, DOWN_W_SIZE);
    load_int32("data/down1.0_bias_int32.txt", down_b, 64);
    load_int32("data/down1.0_multiplier.txt", down_m, 64);
    load_int32("data/down1.0_shift.txt", down_s, 64);
    load_int32("data/down1.0_weight_zp.txt", down_zw, 64);

    load_int32("data/l4_down1_conv_Zx.txt", &down_Zx, 1);
    load_int32("data/l4_down1_conv_Zy.txt", &down_Zy, 1);
    load_int32("data/l4b_down1_relu_Zy.txt", &down_relu_Zy, 1);

    // ===== LOAD RES2 =====
    load_int8 ("data/res2.block.0_weight_int8.txt", res2_w1, RES2_W_SIZE);
    load_int32("data/res2.block.0_bias_int32.txt", res2_b1, 64);
    load_int32("data/res2.block.0_multiplier.txt", res2_m1, 64);
    load_int32("data/res2.block.0_shift.txt", res2_s1, 64);
    load_int32("data/res2.block.0_weight_zp.txt", res2_zw1, 64);

    load_int8 ("data/res2.block.2_weight_int8.txt", res2_w2, RES2_W_SIZE);
    load_int32("data/res2.block.2_bias_int32.txt", res2_b2, 64);
    load_int32("data/res2.block.2_multiplier.txt", res2_m2, 64);
    load_int32("data/res2.block.2_shift.txt", res2_s2, 64);
    load_int32("data/res2.block.2_weight_zp.txt", res2_zw2, 64);

    load_int32("data/l5_res2_conv1_Zx.txt", &res2_conv1_Zx, 1);
    load_int32("data/l5_res2_conv1_Zy.txt", &res2_conv1_Zy, 1);
    load_int32("data/l6_res2_conv2_Zx.txt", &res2_conv2_Zx, 1);
    load_int32("data/l6_res2_conv2_Zy.txt", &res2_conv2_Zy, 1);

    load_float("data/l6_res2_conv2_Sy.txt", &res2_S_main, 1);
    load_float("data/l5_res2_conv1_Sx.txt", &res2_S_skip, 1);
    load_float("data/l7b_res2_relu_out_Sx.txt", &res2_S_add, 1);

    load_int32("data/l6_res2_conv2_Zy.txt", &res2_Z_main, 1);
    load_int32("data/l5_res2_conv1_Zx.txt", &res2_Z_skip, 1);
    load_int32("data/l7b_res2_relu_out_Zx.txt", &res2_Z_add, 1);

    load_int32("data/l7b_res2_relu_out_Zy.txt", &res2_relu_Zy, 1);

    // ===== DEBUG PARAM =====
    printf("Loaded full CNN: stem -> res1 -> down1 -> res2\n");
    printf("Output size = %d\n", RES2_SIZE);

    // ===== RUN =====
    cnn_forward(
        input,
        output,

        stem_w, stem_b, stem_m, stem_s, stem_zw,
        stem_Zx, stem_Zy, stem_relu_Zy,

        res1_w1, res1_b1, res1_m1, res1_s1, res1_zw1,
        res1_w2, res1_b2, res1_m2, res1_s2, res1_zw2,

        res1_conv1_Zx, res1_conv1_Zy,
        res1_conv2_Zx, res1_conv2_Zy,

        res1_S_main, res1_S_skip, res1_S_add,
        res1_Z_main, res1_Z_skip, res1_Z_add,
        res1_relu_Zy,

        down_w, down_b, down_m, down_s, down_zw,
        down_Zx, down_Zy, down_relu_Zy,

        res2_w1, res2_b1, res2_m1, res2_s1, res2_zw1,
        res2_w2, res2_b2, res2_m2, res2_s2, res2_zw2,

        res2_conv1_Zx, res2_conv1_Zy,
        res2_conv2_Zx, res2_conv2_Zy,

        res2_S_main, res2_S_skip, res2_S_add,
        res2_Z_main, res2_Z_skip, res2_Z_add,
        res2_relu_Zy
    );

    // ===== COMPARE =====
    int err = 0;
    int max_diff = 0;
    const int TOL = 30;

    for (int i = 0; i < RES2_SIZE; i++) {
        int diff = abs((int)output[i] - (int)golden[i]);

        if (diff > max_diff)
            max_diff = diff;

        if (diff > CNN_TOL) {
            printf("[WARN] CNN diff exceeds tolerance at %d: got %d, golden %d, diff %d\n",
                i, output[i], golden[i], diff);
            err++;
            if (err > 20)
                break;
        }
    }

    printf("Max diff = %d\n", max_diff);
    printf("Tolerance = %d\n", CNN_TOL);

    if (max_diff <= CNN_TOL)
        printf("PASS CNN: within accepted tolerance\n");
    else
        printf("FAIL CNN: exceeds tolerance\n");

    printf("DONE CNN TEST\n");

    return 0;
}