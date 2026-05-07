#ifndef HLS_QUANT_H
#define HLS_QUANT_H

#include "hls_types.h"

qact_t clamp_u8(qacc_t x);

qacc_t requantize_to_int32(
    qacc_t acc,
    qmult_t multiplier,
    qshift_t shift
);

qact_t requantize_to_u8(
    qacc_t acc,
    qmult_t multiplier,
    qshift_t shift,
    qzp_t Zy
);

#endif