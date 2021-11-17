#ifndef __ALGO_SNAPPYC_H__
#define __ALGO_SNAPPYC_H__

#include "pico/stdlib.h"

void mysnappyc_init();
void mysnappyc_compress(uint8_t* input, size_t insize, uint8_t* output, size_t* outsize);
void mysnappyc_uncompress(uint8_t* input, size_t insize, uint8_t* output, size_t* outsize);

#endif // __ALGO_SNAPPYC_H__
