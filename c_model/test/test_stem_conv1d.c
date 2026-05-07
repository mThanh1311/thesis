#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "quant.h"
#include "load_txt.h"

// ===== DECLARE =====
void conv1d_ref(
    const uint8_t *input,
    const int8_t *weight,
    const int32_t *bias,
    uint8_t *output,

    int in_ch,
    int out_ch,
    int in_len,
    int kernel,
    int stride,
    int padding,

    const int32_t *multiplier,
    const int32_t *shift,

    int32_t Zx,
    const int32_t *Zw,
    int32_t Zy
);

int main() {
    const int TOL = 8;  // acceptable tolerance for quantized conv1d output
    int32_t Zx, Zy;

    // ===== SHAPE =====
    int in_ch = 1;
    int in_len = 320;

    int out_ch = 32;
    int kernel = 5;
    int stride = 2;
    int padding = 2;

    int out_len = (in_len + 2 * padding - kernel) / stride + 1;

    // ===== ALLOC =====
    uint8_t *input  = malloc(in_ch * in_len * sizeof(uint8_t));
    int8_t  *weight = malloc(out_ch * in_ch * kernel * sizeof(int8_t));
    int32_t *bias   = malloc(out_ch * sizeof(int32_t));

    uint8_t *output = malloc(out_ch * out_len * sizeof(uint8_t));
    uint8_t *golden = malloc(out_ch * out_len * sizeof(uint8_t));

    float   *requant_scale = malloc(out_ch * sizeof(float));
    int32_t *Zw = malloc(out_ch * sizeof(int32_t));
    int32_t *multiplier = malloc(out_ch * sizeof(int32_t));
    int32_t *shift      = malloc(out_ch * sizeof(int32_t));


    // ===== LOAD =====
    load_uint8("data/l0_stem_conv_in_u8.txt", input, in_ch * in_len);
    load_uint8("data/l0_stem_conv_out_u8.txt", golden, out_ch * out_len);

    load_int8("data/stem.0_weight_int8.txt", weight, out_ch * in_ch * kernel);
    load_int32("data/stem.0_bias_int32.txt", bias, out_ch);

    load_float("data/stem.0_requant_scale.txt", requant_scale, out_ch);
    load_int32("data/stem.0_weight_zp.txt", Zw, out_ch);

    load_int32("data/l0_stem_conv_Zx.txt", &Zx, 1);
    load_int32("data/l0_stem_conv_Zy.txt", &Zy, 1);

    load_int32("data/stem.0_multiplier.txt", multiplier, out_ch);
    load_int32("data/stem.0_shift.txt", shift, out_ch);

    // ===== DEBUG (optional) =====
    printf("Zx=%d Zy=%d\n", Zx, Zy);
    printf("w[0]=%d\n", weight[0]);
    printf("w[1]=%d\n", weight[1]);
    printf("w[2]=%d\n", weight[2]);

    // ===== RUN =====
    conv1d_ref(
        input, weight, bias, output,
        in_ch, out_ch, in_len,
        kernel, stride, padding,
        multiplier, shift,
        Zx, Zw, Zy
    );

    // ===== COMPARE =====
    int err = 0;
    int max_diff = 0;

    for (int i = 0; i < out_ch * out_len; i++) {

        int diff = abs((int)output[i] - (int)golden[i]);

        // track max diff
        if (diff > max_diff)
            max_diff = diff;

        // print mismatch (optional)
        if (diff > TOL) {
            printf("[WARN] Conv1D diff exceeds tolerance at %d: got %d, golden %d, diff %d\n",
                            i, output[i], golden[i], diff);
            err++;
            if (err > 20) break;
        }
    }
    
    printf("Max diff = %d\n", max_diff);
    printf("Tolerance = %d\n", TOL);

    if (max_diff <= TOL)
        printf("PASS CONV1D: within accepted tolerance\n");
    else
        printf("FAIL CONV1D: exceeds tolerance\n");

    // ===== FREE =====
    free(input);
    free(weight);
    free(bias);
    free(output);
    free(golden);
    free(requant_scale);
    free(Zw);
    free(multiplier);
    free(shift);
    return 0;
}