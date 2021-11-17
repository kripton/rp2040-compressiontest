#ifndef __ALGO_ZLIB_H__
#define __ALGO_ZLIB_H__

#include "pico/stdlib.h"

void zlib_init();
void zlib_compress(uint8_t* input, size_t insize, uint8_t* output, size_t* outsize);
void zlib_uncompress(uint8_t* input, size_t insize, uint8_t* output, size_t* outsize);
void zlib_deinit();

#endif // __ALGO_ZLIB_H__
