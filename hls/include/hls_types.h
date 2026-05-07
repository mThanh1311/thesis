#ifndef HLS_TYPES_H
#define HLS_TYPES_H

#include <stdint.h>

typedef uint8_t  qact_t;   // uint8 activation
typedef int8_t   qwgt_t;   // int8 weight
typedef int32_t  qbias_t;  // int32 bias
typedef int32_t  qmult_t;  // int32 multiplier
typedef int32_t  qshift_t; // int32 shift
typedef int32_t  qzp_t;    // zero-point
typedef int32_t  qacc_t;   // accumulator

#endif