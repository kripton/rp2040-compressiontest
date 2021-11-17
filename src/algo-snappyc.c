#include "algo-zlib.h"

#include "../lib/snappy-c/snappy.h"

struct snappy_env env;

void mysnappyc_init()
{
    snappy_init_env(&env);
}

void mysnappyc_compress(uint8_t *input, size_t insize, uint8_t *output, size_t *outsize)
{
    SNAPPYC_FUNCTION_NAME_PREFIXsnappy_compress(&env, input, insize, output, outsize);
}

void mysnappyc_uncompress(uint8_t *input, size_t insize, uint8_t *output, size_t *outsize)
{
    SNAPPYC_FUNCTION_NAME_PREFIXsnappy_uncompress(input, insize, output);
}
