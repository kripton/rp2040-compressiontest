#include "algo-zlib.h"

#include "../lib/brotli/c/include/brotli/encode.h"
#include "../lib/brotli/c/include/brotli/decode.h"

void brotli_init()
{
}

void brotli_compress(uint8_t *input, size_t insize, uint8_t *output, size_t *outsize)
{
    BrotliEncoderCompress(1, 10, BROTLI_DEFAULT_MODE, insize, input, outsize, output);
}

void brotli_uncompress(uint8_t *input, size_t insize, uint8_t *output, size_t *outsize)
{
    BrotliDecoderDecompress(insize, input, outsize, output);
}

void brotli_deinit()
{
}