#include "algo-zstd.h"

#include "zstd.h"

static struct ZSTD_CCtx_s* cctx;
static struct ZSTD_DCtx_s* dctx;

void zstd_init()
{
    cctx = ZSTD_createCCtx();
    dctx = ZSTD_createDCtx();
}

void zstd_compress(uint8_t *input, size_t insize, uint8_t *output, size_t *outsize)
{
    *outsize = ZSTD_compressCCtx(cctx, output, *outsize, input, insize, 1);
}

void zstd_uncompress(uint8_t *input, size_t insize, uint8_t *output, size_t *outsize)
{
    *outsize = ZSTD_decompressDCtx(dctx, output, *outsize, input, insize);
}

void zstd_deinit()
{
    ZSTD_freeCCtx(cctx);
    ZSTD_freeDCtx(dctx);
}