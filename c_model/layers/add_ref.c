#include <stdint.h>
#include "quant.h"

static int32_t multiply_by_quantized_multiplier(
    int32_t x,
    int32_t multiplier,
    int32_t shift
) {
    int total_shift = 31 - shift;

    int64_t prod = (int64_t)x * (int64_t)multiplier;

    if (total_shift > 0) {
        int64_t rounding = 1LL << (total_shift - 1);
        if (prod >= 0)
            prod += rounding;
        else
            prod -= rounding;

        return (int32_t)(prod >> total_shift);
    } else {
        return (int32_t)(prod << (-total_shift));
    }
}

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
) {
    int32_t M1, M2;
    int32_t sh1, sh2;

    quantize_multiplier(S1 / Sy, &M1, &sh1);
    quantize_multiplier(S2 / Sy, &M2, &sh2);

    for (int i = 0; i < size; i++) {
        int32_t a = (int32_t)x1[i] - Z1;
        int32_t b = (int32_t)x2[i] - Z2;

        int32_t aq = multiply_by_quantized_multiplier(a, M1, sh1);
        int32_t bq = multiply_by_quantized_multiplier(b, M2, sh2);

        int32_t q = aq + bq + Zy;

        if (q < 0) q = 0;
        if (q > 255) q = 255;

        y[i] = (uint8_t)q;
    }
}