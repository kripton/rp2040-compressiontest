#ifndef __ALGO_SNAPPY_H__
#define __ALGO_SNAPPY_H__

#include "pico/stdlib.h"

void mysnappy_init();
void mysnappy_compress(uint8_t* input, size_t insize, uint8_t* output, size_t* outsize);
void mysnappy_uncompress(uint8_t* input, size_t insize, uint8_t* output, size_t* outsize);
void mysnappy_deinit();

#endif // __ALGO_SNAPPY_H__
