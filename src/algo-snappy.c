#include "algo-snappy.h"

#include "../lib/snappy/snappy-c.h"

void mysnappy_init()
{
}

void mysnappy_compress(uint8_t *input, size_t insize, uint8_t *output, size_t *outsize)
{
    snappy_compress(input, insize, output, outsize);
}

void mysnappy_uncompress(uint8_t *input, size_t insize, uint8_t *output, size_t *outsize)
{
    snappy_uncompress(input, insize, output, outsize); 
}
