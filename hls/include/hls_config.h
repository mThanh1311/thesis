#ifndef HLS_CONFIG_H
#define HLS_CONFIG_H

// First HLS target: STEM CONV1D
// C-model constants from stem.c

#define STEM_IN_CH      1
#define STEM_IN_LEN     320
#define STEM_OUT_CH     32
#define STEM_KERNEL     5
#define STEM_STRIDE     2
#define STEM_PADDING    2
#define STEM_OUT_LEN    160

#define STEM_IN_SIZE    (STEM_IN_CH * STEM_IN_LEN)
#define STEM_OUT_SIZE   (STEM_OUT_CH * STEM_OUT_LEN)
#define STEM_W_SIZE     (STEM_KERNEL * STEM_IN_CH * STEM_OUT_CH)
#define STEM_B_SIZE     (STEM_OUT_CH)

#define RES_IN_CH      STEM_OUT_CH
#define RES_IN_LEN     STEM_OUT_LEN
#define RES_OUT_CH     STEM_OUT_CH
#define RES_KERNEL     3
#define RES_STRIDE     1
#define RES_PADDING    1
#define RES_OUT_LEN    STEM_OUT_LEN

#define RES_IN_SIZE    (RES_IN_CH * RES_IN_LEN)
#define RES_OUT_SIZE   (RES_OUT_CH * RES_OUT_LEN)
#define RES_W_SIZE     (RES_KERNEL * RES_IN_CH * RES_OUT_CH)
#define RES_B_SIZE     (RES_OUT_CH)

// ============================================================
// DOWNSAMPLE HLS CONFIG
// ============================================================

#define DOWN_IN_CH      RES_OUT_CH
#define DOWN_IN_LEN     RES_OUT_LEN

#define DOWN_OUT_CH     64
#define DOWN_KERNEL     3
#define DOWN_STRIDE     2
#define DOWN_PADDING    1

#define DOWN_OUT_LEN    (DOWN_IN_LEN / 2)

#define DOWN_IN_SIZE    (DOWN_IN_CH * DOWN_IN_LEN)
#define DOWN_OUT_SIZE   (DOWN_OUT_CH * DOWN_OUT_LEN)

#define DOWN_W_SIZE     (DOWN_KERNEL * DOWN_IN_CH * DOWN_OUT_CH)
#define DOWN_B_SIZE     (DOWN_OUT_CH)

// ============================================================
// RESBLOCK2 HLS CONFIG
// ============================================================

#define RES2_IN_CH      DOWN_OUT_CH
#define RES2_IN_LEN     DOWN_OUT_LEN
#define RES2_OUT_CH     DOWN_OUT_CH
#define RES2_KERNEL     3
#define RES2_STRIDE     1
#define RES2_PADDING    1
#define RES2_OUT_LEN    DOWN_OUT_LEN

#define RES2_IN_SIZE    (RES2_IN_CH * RES2_IN_LEN)
#define RES2_OUT_SIZE   (RES2_OUT_CH * RES2_OUT_LEN)
#define RES2_W_SIZE     (RES2_KERNEL * RES2_IN_CH * RES2_OUT_CH)
#define RES2_B_SIZE     (RES2_OUT_CH)


#endif