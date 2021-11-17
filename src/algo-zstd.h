#ifndef __ALGO_ZSTD_H__
#define __ALGO_ZSTD_H__

#include "pico/stdlib.h"

void zstd_init();
void zstd_compress(uint8_t* input, size_t insize, uint8_t* output, size_t* outsize);
void zstd_uncompress(uint8_t* input, size_t insize, uint8_t* output, size_t* outsize);
void zstd_deinit();

#endif // __ALGO_ZSTD_H__
