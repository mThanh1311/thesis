#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "load_txt.h"

void add_ref(
    const uint8_t *x1,
    const uint8_t *x2,
    uint8_t *y,
    int size,

    float S1,
    float S2,
    float Sy,

    int32_t Z1,
    int32_t Z2,
    int32_t Zy
);

int main() {

    const int TOL = 10;  // acceptable tolerance for quantized addition
    int ch = 32;
    int len = 160;
    int size = ch * len;

    uint8_t *x1 = malloc(size * sizeof(uint8_t));
    uint8_t *x2 = malloc(size * sizeof(uint8_t));
    uint8_t *y  = malloc(size * sizeof(uint8_t));
    uint8_t *golden = malloc(size * sizeof(uint8_t));

    float S1, S2, Sy;
    int32_t Z1, Z2, Zy;

    // main branch: output của conv2
    load_uint8("data/l2_res1_conv2_out_u8.txt", x1, size);
    load_float("data/l2_res1_conv2_Sy.txt", &S1, 1);
    load_int32("data/l2_res1_conv2_Zy.txt", &Z1, 1);

    // skip branch: input ban đầu của residual block
    load_uint8("data/l1_res1_conv1_in_u8.txt", x2, size);
    load_float("data/l1_res1_conv1_Sx.txt", &S2, 1);
    load_int32("data/l1_res1_conv1_Zx.txt", &Z2, 1);

    // output add = input của ReLU tiếp theo
    load_float("data/l3b_res1_relu_out_Sx.txt", &Sy, 1);
    load_int32("data/l3b_res1_relu_out_Zx.txt", &Zy, 1);

    // golden add output
    load_uint8("data/l3b_res1_relu_out_in_u8.txt", golden, size);

    add_ref(
        x1, x2, y, size,
        S1, S2, Sy,
        Z1, Z2, Zy
    );

    int err = 0;
    int max_diff = 0;

    for (int i = 0; i < size; i++) {
        int diff = abs((int)y[i] - (int)golden[i]);

        if (diff > max_diff)
            max_diff = diff;

        if (diff > TOL) {
            printf("[WARN] Add diff exceeds tolerance at %d: got %d, golden %d, diff %d\n",
                i, y[i], golden[i], diff);
            err++;
            if (err > 20) break;
        }
    }

    printf("Max diff = %d\n", max_diff);

    printf("Tolerance = %d\n", TOL);

    if (max_diff <= TOL)
        printf("PASS ADD: within accepted tolerance\n");
    else
        printf("FAIL ADD: exceeds tolerance\n");

    printf("DONE ADD TEST\n");

    free(x1);
    free(x2);
    free(y);
    free(golden);

    return 0;
}