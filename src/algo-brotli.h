#ifndef __ALGO_BROTLI_H__
#define __ALGO_BROTLI_H__

#include "pico/stdlib.h"

void brotli_init();
void brotli_compress(uint8_t* input, size_t insize, uint8_t* output, size_t* outsize);
void brotli_uncompress(uint8_t* input, size_t insize, uint8_t* output, size_t* outsize);
void brotli_deinit();

#endif // __ALGO_BROTLI_H__
