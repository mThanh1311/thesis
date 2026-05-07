#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "load_txt.h"

// declare
void relu_ref(
    const uint8_t *input,
    uint8_t *output,
    int size,
    int32_t Zy
);

int main() {

    // ===== SHAPE =====
    int size = 32 * 160;   // giống output conv

    // ===== ALLOC =====
    uint8_t *input  = malloc(size * sizeof(uint8_t));
    uint8_t *output = malloc(size * sizeof(uint8_t));
    uint8_t *golden = malloc(size * sizeof(uint8_t));

    int32_t Zy;

    // ===== LOAD =====
    load_uint8("data/l0b_stem_relu_in_u8.txt", input, size);
    load_uint8("data/l0b_stem_relu_out_u8.txt", golden, size);

    load_int32("data/l0b_stem_relu_Zy.txt", &Zy, 1);

    printf("Zy = %d\n", Zy);

    // ===== RUN =====
    relu_ref(input, output, size, Zy);

    // ===== COMPARE =====
    int err = 0;

    for (int i = 0; i < size; i++) {

        if (output[i] != golden[i]) {
            printf("Mismatch at %d: %d vs %d\n",
                   i, output[i], golden[i]);
            err++;
            if (err > 20) break;
        }
    }

    // if (err == 0)
    //     printf("PASS RELU\n");
    // else
    //     printf("FAIL (%d errors)\n", err);

    printf("DONE RELU TEST\n");

    // ===== FREE =====
    free(input);
    free(output);
    free(golden);

    return 0;
}