#ifndef __ALGO_HEATSHRINK_H__
#define __ALGO_HEATSHRINK_H__

#include "pico/stdlib.h"

void heatshrink_init();
void heatshrink_compress(uint8_t* input, size_t insize, uint8_t* output, size_t* outsize);
void heatshrink_uncompress(uint8_t* input, size_t insize, uint8_t* output, size_t* outsize);

#endif // __ALGO_HEATSHRINK_H__
