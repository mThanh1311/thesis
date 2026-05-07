#include <stdint.h>
#include <math.h>
#include <stdio.h>

void fc_fp32_ref(
    const float *input,
    const float *weight,
    const float *bias,
    float *output,
    int in_features,
    int out_features
) {
    for (int oc = 0; oc < out_features; oc++) {
        float acc = bias[oc];

        for (int i = 0; i < in_features; i++) {
            acc += input[i] * weight[oc * in_features + i];
        }

        output[oc] = acc;
    }
}