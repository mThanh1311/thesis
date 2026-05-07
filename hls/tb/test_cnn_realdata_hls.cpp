#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "cnn_realdata_hls.h"

#define DATA_DIR "D:/Thesis/workspaces/HLS01/hls_realdata/"

static int load_int_file(const char *path, int *buf, int n) {
    FILE *f = fopen(path, "r");
    if (!f) {
        printf("ERROR: cannot open %s\n", path);
        return -1;
    }

    for (int i = 0; i < n; i++) {
        if (fscanf(f, "%d", &buf[i]) != 1) {
            printf("ERROR reading %s at index %d\n", path, i);
            fclose(f);
            return -1;
        }
    }

    fclose(f);
    return 0;
}

template <typename T>
static int load_cast(const char *path, T *dst, int n) {
    int *tmp = (int *)malloc(sizeof(int) * n);
    if (!tmp) {
        printf("ERROR: malloc failed for %s\n", path);
        return -1;
    }

    int ret = load_int_file(path, tmp, n);

    if (ret == 0) {
        for (int i = 0; i < n; i++) {
            dst[i] = (T)tmp[i];
        }
    }

    free(tmp);
    return ret;
}

int main() {
    static qact_t input[STEM_IN_SIZE];
    static qact_t expected[RES2_OUT_SIZE];
    static qact_t actual[RES2_OUT_SIZE];

    static qwgt_t stem_w[STEM_W_SIZE];
    static qbias_t stem_b[STEM_B_SIZE];
    static qmult_t stem_m[STEM_OUT_CH];
    static qshift_t stem_s[STEM_OUT_CH];
    static qzp_t stem_wzp[STEM_OUT_CH];

    static qwgt_t res1_w1[RES_W_SIZE];
    static qbias_t res1_b1[RES_B_SIZE];
    static qmult_t res1_m1[RES_OUT_CH];
    static qshift_t res1_s1[RES_OUT_CH];
    static qzp_t res1_wzp1[RES_OUT_CH];

    static qwgt_t res1_w2[RES_W_SIZE];
    static qbias_t res1_b2[RES_B_SIZE];
    static qmult_t res1_m2[RES_OUT_CH];
    static qshift_t res1_s2[RES_OUT_CH];
    static qzp_t res1_wzp2[RES_OUT_CH];

    static qwgt_t down_w[DOWN_W_SIZE];
    static qbias_t down_b[DOWN_B_SIZE];
    static qmult_t down_m[DOWN_OUT_CH];
    static qshift_t down_s[DOWN_OUT_CH];
    static qzp_t down_wzp[DOWN_OUT_CH];

    static qwgt_t res2_w1[RES2_W_SIZE];
    static qbias_t res2_b1[RES2_B_SIZE];
    static qmult_t res2_m1[RES2_OUT_CH];
    static qshift_t res2_s1[RES2_OUT_CH];
    static qzp_t res2_wzp1[RES2_OUT_CH];

    static qwgt_t res2_w2[RES2_W_SIZE];
    static qbias_t res2_b2[RES2_B_SIZE];
    static qmult_t res2_m2[RES2_OUT_CH];
    static qshift_t res2_s2[RES2_OUT_CH];
    static qzp_t res2_wzp2[RES2_OUT_CH];

    int sc[26];

    printf("========================================\n");
    printf("Running CNN real-data HLS validation...\n");
    printf("DATA_DIR = %s\n", DATA_DIR);
    printf("========================================\n");

    if (load_cast(DATA_DIR "input.txt", input, STEM_IN_SIZE)) return 1;
    if (load_cast(DATA_DIR "cnn_expected.txt", expected, RES2_OUT_SIZE)) return 1;

    if (load_cast(DATA_DIR "stem_w.txt", stem_w, STEM_W_SIZE)) return 1;
    if (load_cast(DATA_DIR "stem_b.txt", stem_b, STEM_B_SIZE)) return 1;
    if (load_cast(DATA_DIR "stem_m.txt", stem_m, STEM_OUT_CH)) return 1;
    if (load_cast(DATA_DIR "stem_s.txt", stem_s, STEM_OUT_CH)) return 1;
    if (load_cast(DATA_DIR "stem_wzp.txt", stem_wzp, STEM_OUT_CH)) return 1;

    if (load_cast(DATA_DIR "res1_w1.txt", res1_w1, RES_W_SIZE)) return 1;
    if (load_cast(DATA_DIR "res1_b1.txt", res1_b1, RES_B_SIZE)) return 1;
    if (load_cast(DATA_DIR "res1_m1.txt", res1_m1, RES_OUT_CH)) return 1;
    if (load_cast(DATA_DIR "res1_s1.txt", res1_s1, RES_OUT_CH)) return 1;
    if (load_cast(DATA_DIR "res1_wzp1.txt", res1_wzp1, RES_OUT_CH)) return 1;

    if (load_cast(DATA_DIR "res1_w2.txt", res1_w2, RES_W_SIZE)) return 1;
    if (load_cast(DATA_DIR "res1_b2.txt", res1_b2, RES_B_SIZE)) return 1;
    if (load_cast(DATA_DIR "res1_m2.txt", res1_m2, RES_OUT_CH)) return 1;
    if (load_cast(DATA_DIR "res1_s2.txt", res1_s2, RES_OUT_CH)) return 1;
    if (load_cast(DATA_DIR "res1_wzp2.txt", res1_wzp2, RES_OUT_CH)) return 1;

    if (load_cast(DATA_DIR "down_w.txt", down_w, DOWN_W_SIZE)) return 1;
    if (load_cast(DATA_DIR "down_b.txt", down_b, DOWN_B_SIZE)) return 1;
    if (load_cast(DATA_DIR "down_m.txt", down_m, DOWN_OUT_CH)) return 1;
    if (load_cast(DATA_DIR "down_s.txt", down_s, DOWN_OUT_CH)) return 1;
    if (load_cast(DATA_DIR "down_wzp.txt", down_wzp, DOWN_OUT_CH)) return 1;

    if (load_cast(DATA_DIR "res2_w1.txt", res2_w1, RES2_W_SIZE)) return 1;
    if (load_cast(DATA_DIR "res2_b1.txt", res2_b1, RES2_B_SIZE)) return 1;
    if (load_cast(DATA_DIR "res2_m1.txt", res2_m1, RES2_OUT_CH)) return 1;
    if (load_cast(DATA_DIR "res2_s1.txt", res2_s1, RES2_OUT_CH)) return 1;
    if (load_cast(DATA_DIR "res2_wzp1.txt", res2_wzp1, RES2_OUT_CH)) return 1;

    if (load_cast(DATA_DIR "res2_w2.txt", res2_w2, RES2_W_SIZE)) return 1;
    if (load_cast(DATA_DIR "res2_b2.txt", res2_b2, RES2_B_SIZE)) return 1;
    if (load_cast(DATA_DIR "res2_m2.txt", res2_m2, RES2_OUT_CH)) return 1;
    if (load_cast(DATA_DIR "res2_s2.txt", res2_s2, RES2_OUT_CH)) return 1;
    if (load_cast(DATA_DIR "res2_wzp2.txt", res2_wzp2, RES2_OUT_CH)) return 1;

    if (load_int_file(DATA_DIR "scalars.txt", sc, 26)) return 1;

    cnn_realdata_hls(
        input,

        stem_w,
        stem_b,
        stem_m,
        stem_s,
        stem_wzp,

        res1_w1,
        res1_b1,
        res1_m1,
        res1_s1,
        res1_wzp1,

        res1_w2,
        res1_b2,
        res1_m2,
        res1_s2,
        res1_wzp2,

        sc[12], sc[13], sc[14],
        sc[15], sc[16], sc[17],
        sc[18],

        down_w,
        down_b,
        down_m,
        down_s,
        down_wzp,

        res2_w1,
        res2_b1,
        res2_m1,
        res2_s1,
        res2_wzp1,

        res2_w2,
        res2_b2,
        res2_m2,
        res2_s2,
        res2_wzp2,

        sc[19], sc[20], sc[21],
        sc[22], sc[23], sc[24],
        sc[25],

        actual,

        sc[0], sc[1],
        sc[2], sc[3],
        sc[4], sc[5],
        sc[6], sc[7],
        sc[8], sc[9],
        sc[10], sc[11]
    );

    int max_diff = 0;
    int mismatch = 0;
    const int tolerance = 20;

    for (int i = 0; i < RES2_OUT_SIZE; i++) {
        int diff = (int)actual[i] - (int)expected[i];
        if (diff < 0) diff = -diff;

        if (diff > max_diff) {
            max_diff = diff;
        }

        if (diff > tolerance) {
            mismatch++;

            if (mismatch <= 30) {
                printf(
                    "[Mismatch] idx=%d expected=%u actual=%u diff=%d\n",
                    i,
                    expected[i],
                    actual[i],
                    diff
                );
            }
        }
    }

    printf("========================================\n");
    printf("CNN real-data HLS result\n");
    printf("max_diff          = %d\n", max_diff);
    printf("mismatch_over_tol = %d\n", mismatch);
    printf("tolerance         = %d\n", tolerance);
    printf("========================================\n");

    if (mismatch == 0) {
        printf("PASS\n");
        return 0;
    }

    printf("FAIL\n");
    return 1;
}