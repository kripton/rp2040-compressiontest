#include "algo-heatshrink.h"

#include "../lib/heatshrink/heatshrink_common.h"
#include "../lib/heatshrink/heatshrink_encoder.h"
#include "../lib/heatshrink/heatshrink_decoder.h"

static heatshrink_decoder dec;
static heatshrink_encoder enc;

void heatshrink_init()
{
    heatshrink_decoder_reset(&dec);
    heatshrink_encoder_reset(&enc);
}

void heatshrink_compress(uint8_t *input, size_t insize, uint8_t *output, size_t *outsize)
{
    heatshrink_encoder_reset(&enc);

    size_t sunk = 0;
    size_t polled = 0;
    size_t count = 0;
    size_t comp_sz = *outsize;
    while (sunk < insize) {
        heatshrink_encoder_sink(&enc, &input[sunk], insize - sunk, &count);
        sunk += count;
        if (sunk == insize) {
            heatshrink_encoder_finish(&enc);
        }

        HSE_poll_res pres;
        do {
            pres = heatshrink_encoder_poll(&enc, &output[polled], comp_sz - polled, &count);
            polled += count;
        } while (pres == HSER_POLL_MORE);
        if (sunk == insize) {
            heatshrink_encoder_finish(&enc);
        }
    }

    *outsize = polled;
}
