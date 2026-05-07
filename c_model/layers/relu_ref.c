#include <stdio.h>
#include <stdint.h>

void relu_ref(
    const uint8_t *input,
    uint8_t *output,
    int size,
    int32_t Zy
) {
    for (int i = 0; i < size; i++) {

        uint8_t x = input[i];

        // ReLU in quant domain
        if (x < Zy)
            output[i] = (uint8_t)Zy;
        else
            output[i] = x;
    }
}