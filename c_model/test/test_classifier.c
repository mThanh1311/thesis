#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#include "load_txt.h"

#define IN_FEATURES  64
#define OUT_FEATURES 5

void fc_fp32_ref(
    const float *input,
    const float *weight,
    const float *bias,
    float *output,
    int in_features,
    int out_features
);

int main() {
    const float TOL = 0.1f;

    float *fc_in  = malloc(IN_FEATURES * sizeof(float));
    float *weight = malloc(OUT_FEATURES * IN_FEATURES * sizeof(float));
    float *bias   = malloc(OUT_FEATURES * sizeof(float));
    float *output = malloc(OUT_FEATURES * sizeof(float));
    float *golden = malloc(OUT_FEATURES * sizeof(float));

    load_float("data/q2_fc_quant_in_fp32.txt", fc_in, IN_FEATURES);
    load_float("data/fc_weight.txt", weight, OUT_FEATURES * IN_FEATURES);
    load_float("data/fc_bias.txt", bias, OUT_FEATURES);
    load_float("data/model_output_fp32.txt", golden, OUT_FEATURES);

    fc_fp32_ref(
        fc_in,
        weight,
        bias,
        output,
        IN_FEATURES,
        OUT_FEATURES
    );

    float max_abs_err = 0.0f;

    for (int i = 0; i < OUT_FEATURES; i++) {
        float err = fabsf(output[i] - golden[i]);

        if (err > max_abs_err)
            max_abs_err = err;

        printf("out[%d] = %f, golden = %f, err = %f\n",
               i, output[i], golden[i], err);
    }

    printf("FC FP32 max abs error = %f\n", max_abs_err);
    printf("Tolerance = %f\n", TOL);

    if (max_abs_err <= TOL)
        printf("PASS CLASSIFIER: within accepted tolerance\n");
    else
        printf("FAIL CLASSIFIER: exceeds tolerance\n");

    printf("DONE CLASSIFIER TEST\n");

    free(fc_in);
    free(weight);
    free(bias);
    free(output);
    free(golden);

    return 0;
}