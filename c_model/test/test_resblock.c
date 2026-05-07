#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "load_txt.h"
#include "resblock.h"

#define CH   32
#define LEN  160
#define SIZE (CH * LEN)

int main() {

    const int CONV2_TOL = 12;

    // ===== ALLOC =====
    uint8_t *input  = malloc(SIZE);
    uint8_t *output = malloc(SIZE);
    uint8_t *golden = malloc(SIZE);

    uint8_t *buf1 = malloc(SIZE);
    uint8_t *buf2 = malloc(SIZE);

    // ===== CONV1 =====
    int8_t  *w1 = malloc(CH * CH * 5);
    int32_t *b1 = malloc(CH * sizeof(int32_t));
    int32_t *m1 = malloc(CH * sizeof(int32_t));
    int32_t *s1 = malloc(CH * sizeof(int32_t));
    int32_t *zw1 = malloc(CH * sizeof(int32_t));

    // ===== CONV2 =====
    int8_t  *w2 = malloc(CH * CH * 5);
    int32_t *b2 = malloc(CH * sizeof(int32_t));
    int32_t *m2 = malloc(CH * sizeof(int32_t));
    int32_t *s2 = malloc(CH * sizeof(int32_t));
    int32_t *zw2 = malloc(CH * sizeof(int32_t));

    // ===== SCALE =====
    float S_main, S_skip, S_add;
    int32_t Z_main, Z_skip, Z_add;

    int32_t conv1_Zx, conv1_Zy;
    int32_t conv2_Zx, conv2_Zy;
    int32_t relu_Zy;

    // ===== LOAD INPUT =====
    load_uint8("data/l1_res1_conv1_in_u8.txt", input, SIZE);

    // ===== LOAD CONV1 =====
    load_int8("data/res1.block.0_weight_int8.txt", w1, CH * CH * 5);
    load_int32("data/res1.block.0_bias_int32.txt", b1, CH);
    load_int32("data/res1.block.0_multiplier.txt", m1, CH);
    load_int32("data/res1.block.0_shift.txt", s1, CH);
    load_int32("data/res1.block.0_weight_zp.txt", zw1, CH);

    load_int32("data/l1_res1_conv1_Zx.txt", &conv1_Zx, 1);
    load_int32("data/l1_res1_conv1_Zy.txt", &conv1_Zy, 1);

    // ===== LOAD CONV2 =====
    load_int8("data/res1.block.2_weight_int8.txt", w2, CH * CH * 5);
    load_int32("data/res1.block.2_bias_int32.txt", b2, CH);
    load_int32("data/res1.block.2_multiplier.txt", m2, CH);
    load_int32("data/res1.block.2_shift.txt", s2, CH);
    load_int32("data/res1.block.2_weight_zp.txt", zw2, CH);

    load_int32("data/l2_res1_conv2_Zx.txt", &conv2_Zx, 1);
    load_int32("data/l2_res1_conv2_Zy.txt", &conv2_Zy, 1);


    // ===== ADD SCALE =====
    load_float("data/l2_res1_conv2_Sy.txt", &S_main, 1);
    load_float("data/l1_res1_conv1_Sx.txt", &S_skip, 1);
    load_float("data/l3b_res1_relu_out_Sx.txt", &S_add, 1);

    load_int32("data/l2_res1_conv2_Zy.txt", &Z_main, 1);
    load_int32("data/l1_res1_conv1_Zx.txt", &Z_skip, 1);
    load_int32("data/l3b_res1_relu_out_Zx.txt", &Z_add, 1);

    // ===== RELU FINAL =====
    load_int32("data/l3b_res1_relu_out_Zy.txt", &relu_Zy, 1);
    
    // ===== GOLDEN =====
    load_uint8("data/l3b_res1_relu_out_out_u8.txt", golden, SIZE);

    // ===== RUN =====
    resblock_forward(
        input, output,
        buf1, buf2,

        w1, b1, m1, s1, zw1,
        w2, b2, m2, s2, zw2,

        conv1_Zx, conv1_Zy,
        conv2_Zx, conv2_Zy,

        S_main, S_skip, S_add,
        Z_main, Z_skip, Z_add,

        relu_Zy
    );

    // ===== DEBUG CONV2 SAU RUN =====
    uint8_t *golden_conv2 = malloc(SIZE);
    load_uint8("data/l2_res1_conv2_out_u8.txt", golden_conv2, SIZE);

    int err2 = 0;
    int max_diff2 = 0;

    for (int i = 0; i < SIZE; i++) {
        int diff = abs((int)buf2[i] - (int)golden_conv2[i]);

        if (diff > max_diff2) max_diff2 = diff;

        if (diff > CONV2_TOL) {
            printf("[WARN] Conv2 diff exceeds tolerance at %d: got %d, golden %d, diff %d\n",
                i, buf2[i], golden_conv2[i], diff);
            err2++;
            if (err2 > 20) break;
        }
    }

    printf("Conv2 Max diff = %d\n", max_diff2);

    
    // ===== COMPARE =====
    const int RES_TOL = 12;  // acceptable tolerance for quantized resblock output
    int err = 0;
    int max_diff = 0;

    for (int i = 0; i < SIZE; i++) {

        int diff = abs((int)output[i] - (int)golden[i]);

        if (diff > max_diff)
            max_diff = diff;

        if (diff > RES_TOL) {
            printf("[WARN] ResBlock diff exceeds tolerance at %d: got %d, golden %d, diff %d\n",
                i, output[i], golden[i], diff);
            err++;
            if (err > 20) break;
        }
    }

    printf("Max diff = %d\n", max_diff);
    printf("Tolerance = %d\n", RES_TOL);

    if (max_diff <= RES_TOL)
        printf("PASS RESBLOCK: within accepted tolerance\n");
    else
        printf("FAIL RESBLOCK: exceeds tolerance\n");

    printf("DONE RESBLOCK TEST\n");

    return 0;
}