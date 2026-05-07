#include "hls_quant.h"

qact_t clamp_u8(qacc_t x) {
    if (x < 0) return 0;
    if (x > 255) return 255;
    return (qact_t)x;
}

qacc_t requantize_to_int32(
    qacc_t acc,
    qmult_t multiplier,
    qshift_t shift
) {
    int64_t scaled = (int64_t)acc * (int64_t)multiplier;

    int total_shift = 31 + shift;

    if (total_shift > 0) {
        int64_t rounding = 1LL << (total_shift - 1);

        if (scaled >= 0)
            scaled += rounding;
        else
            scaled -= rounding;
    }

    return (qacc_t)(scaled >> total_shift);
}

qact_t requantize_to_u8(
    qacc_t acc,
    qmult_t multiplier,
    qshift_t shift,
    qzp_t Zy
) {
    qacc_t q = requantize_to_int32(acc, multiplier, shift) + Zy;
    return clamp_u8(q);
}