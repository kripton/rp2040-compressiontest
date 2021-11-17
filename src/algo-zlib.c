#include "algo-zlib.h"

#include "../lib/zlib/zlib.h"

void zlib_init()
{
}

void zlib_compress(uint8_t *input, size_t insize, uint8_t *output, size_t *outsize)
{
    long unsigned int outsize2 = *outsize;
    compress2(output, &outsize2, input, insize, 4);
    *outsize = outsize2;
}

void zlib_uncompress(uint8_t *input, size_t insize, uint8_t *output, size_t *outsize)
{
    long unsigned int outsize2 = *outsize;
    uncompress(output, &outsize2, input, insize);
}

void zlib_deinit()
{
}