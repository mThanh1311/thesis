#include "load_txt.h"
#include <stdio.h>

void load_int8(const char *file, int8_t *data, int size) {
    FILE *f = fopen(file, "r");
    for (int i = 0; i < size; i++) {
        int tmp;
        fscanf(f, "%d", &tmp);
        data[i] = (int8_t)tmp;
    }
    fclose(f);
}

void load_int32(const char *file, int32_t *data, int size) {
    FILE *f = fopen(file, "r");
    for (int i = 0; i < size; i++) {
        fscanf(f, "%d", &data[i]);
    }
    fclose(f);
}

void load_float(const char *file, float *data, int size) {
    FILE *f = fopen(file, "r");
    for (int i = 0; i < size; i++) {
        fscanf(f, "%f", &data[i]);
    }
    fclose(f);
}

void load_uint8(const char *file, uint8_t *data, int size) {
    FILE *f = fopen(file, "r");

    if (!f) {
        printf("Cannot open file: %s\n", file);
        return;
    }

    for (int i = 0; i < size; i++) {
        int tmp;
        fscanf(f, "%d", &tmp);
        data[i] = (uint8_t)tmp; 
    }

    fclose(f);
}