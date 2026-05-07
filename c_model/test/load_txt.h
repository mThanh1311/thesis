#ifndef LOAD_TXT_H
#define LOAD_TXT_H

#include <stdint.h>

void load_int8(const char *file, int8_t *data, int size);
void load_int32(const char *file, int32_t *data, int size);
void load_float(const char *file, float *data, int size);
void load_uint8(const char *file, uint8_t *data, int size);


#endif // LOAD_TXT_H