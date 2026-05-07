#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "load_txt.h"
#include "stem.h"

#define STEM_IN_SIZE  320
#define STEM_OUT_SIZE (32 * 160)

int main() {
    const int TOL = 8;  // acceptable tolerance for quantized stem output
    uint8_t *input  = malloc(STEM_IN_SIZE * sizeof(uint8_t));
    uint8_t *output = malloc(STEM_OUT_SIZE * sizeof(uint8_t));
    uint8_t *golden = malloc(STEM_OUT_SIZE * sizeof(uint8_t));

    int8_t  *weight = malloc(32 * 1 * 5 * sizeof(int8_t));
    int32_t *bias   = malloc(32 * sizeof(int32_t));

    int32_t *multiplier = malloc(32 * sizeof(int32_t));
    int32_t *shift      = malloc(32 * sizeof(int32_t));
    int32_t *weight_zp  = malloc(32 * sizeof(int32_t));

    int32_t conv_Zx, conv_Zy, relu_Zy;

    load_uint8("data/l0_stem_conv_in_u8.txt", input, STEM_IN_SIZE);

    load_int8("data/stem.0_weight_int8.txt", weight, 32 * 1 * 5);
    load_int32("data/stem.0_bias_int32.txt", bias, 32);
    load_int32("data/stem.0_multiplier.txt", multiplier, 32);
    load_int32("data/stem.0_shift.txt", shift, 32);
    load_int32("data/stem.0_weight_zp.txt", weight_zp, 32);

    load_int32("data/l0_stem_conv_Zx.txt", &conv_Zx, 1);
    load_int32("data/l0_stem_conv_Zy.txt", &conv_Zy, 1);

    load_int32("data/l0b_stem_relu_Zy.txt", &relu_Zy, 1);

    load_uint8("data/l0b_stem_relu_out_u8.txt", golden, STEM_OUT_SIZE);

    stem_forward(
        input,
        output,
        weight,
        bias,
        multiplier,
        shift,
        weight_zp,
        conv_Zx,
        conv_Zy,
        relu_Zy
    );

    int err = 0;
    int max_diff = 0;

    for (int i = 0; i < STEM_OUT_SIZE; i++) {
        int diff = abs((int)output[i] - (int)golden[i]);

        if (diff > max_diff)
            max_diff = diff;

        if (diff > TOL) {
            printf("[WARN] Stem diff exceeds tolerance at %d: got %d, golden %d, diff %d\n",
                i, output[i], golden[i], diff);
            err++;
            if (err > 20) break;
        }
    }

    printf("Max diff = %d\n", max_diff);
    printf("Tolerance = %d\n", TOL);

    if (max_diff <= TOL)
        printf("PASS STEM: within accepted tolerance\n");
    else
        printf("FAIL STEM: exceeds tolerance\n");

    printf("DONE STEM TEST\n");

    free(input);
    free(output);
    free(golden);
    free(weight);
    free(bias);
    free(multiplier);
    free(shift);
    free(weight_zp);

    return 0;
}