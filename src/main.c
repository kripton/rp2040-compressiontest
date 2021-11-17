/**
 * SPDX-License-Identifier: GPL-3.0
 */

#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "pico/time.h"
#include "tusb.h"

#include "hardware/pll.h"
#include "hardware/clocks.h"
#include "hardware/structs/pll.h"
#include "hardware/structs/clocks.h"

#include "picotool_binary_information.h"

#include "algo-heatshrink.h"
#include "algo-zlib.h"
#if HAVE_ALGO_ZSTD
#include "algo-zstd.h"
#endif
#include "algo-snappy.h"
#include "algo-snappyc.h"
#if HAVE_ALGO_BROTLI
#include "algo-brotli.h"
#endif


// TEST VECTORS
struct testVec {
    uint8_t         name[32];    // Human-readable name
    size_t          size;
    uint8_t         data[512];
};

enum {
    TESTVEC_ALL_ZERO,
    TESTVEC_ALL_ONE,
    TESTVEC_SAWTOOTH_SMALL,
    TESTVEC_SAWTOOTH_MEDIUM,
    TESTVEC_SAWTOOTH_LARGE,
    TESTVEC_ALL_RANDOM,
    TESTVEC_ALTERNATING_RANDOM_ZERO,
    TESTVEC_BLOCKS_8RANDOM_8ZERO,
    // Add more HERE
    TESTVEC_NUM_TOTAL
};

struct testVec testvecs[TESTVEC_NUM_TOTAL];

// Compression algorithms
struct algo {
    uint8_t             name[32];    // Human-readable name
    void (*init)();
    void (*compress)(uint8_t* input, size_t insize, uint8_t* output, size_t* outsize);
    void (*uncompress)(uint8_t* input, size_t insize, uint8_t* output, size_t* outsize);
};

enum {
    ALGO_HEATSHRINK,
    ALGO_ZLIB,
#if HAVE_ALGO_ZSTD
    ALGO_ZSTD,
#endif
    ALGO_SNAPPY,
    ALGO_SNAPPYC,
#if HAVE_ALGO_BROTLI
    ALGO_BROTLI,
#endif
    // Add more HERE
    ALGO_NUM_TOTAL
};

struct algo algos[ALGO_NUM_TOTAL];

// Variables used for elapsed time calculation
uint32_t start_time, finish_time, elapsed_time, elapsed_time_2;

uint16_t i;
uint8_t j;

// Buffer to store compressed and verification data temporarily
#define OUTBUF_SIZE 1024
uint8_t     outbuf[OUTBUF_SIZE];
uint8_t     outbuf_2[OUTBUF_SIZE];

int main() {
    // Overclock the board to 250MHz. According to
    // https://www.youtube.com/watch?v=G2BuoFNLo this should be
    // totally safe with the default 1.10V Vcore
    set_sys_clock_khz(250000, true);

    // Enable the USB console
    stdio_usb_init();

    while (!tud_cdc_connected()) {
        sleep_ms(100);
    }

    uint f_pll_sys = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_PLL_SYS_CLKSRC_PRIMARY);

    printf("Welcome to rp2040-compressiontest\n");
    printf("pll_sys  = %dkHz (= ~%dMHz)\n\n", f_pll_sys, f_pll_sys / 1000);

    ////////////////////////////////////////////////////
    // INIT TEST VECTORS ///////////////////////////////
    ////////////////////////////////////////////////////
    printf("Initializing test vectors ... ");
    start_time = time_us_32();

    // Test vector 0: All zeroes
    sprintf(testvecs[TESTVEC_ALL_ZERO].name, "ALL_ZERO");
    testvecs[TESTVEC_ALL_ZERO].size = 512;
    memset(testvecs[TESTVEC_ALL_ZERO].data, 0, 512);

    // Test vector 1: All ones
    sprintf(testvecs[TESTVEC_ALL_ONE].name, "ALL_ONE");
    testvecs[TESTVEC_ALL_ONE].size = 512;
    memset(testvecs[TESTVEC_ALL_ONE].data, 0xff, 512);

    // Test vector 2: Very small sawtooth: 0, 127, 255, 0, ....
    sprintf(testvecs[TESTVEC_SAWTOOTH_SMALL].name, "SAWTOOTH_SMALL");
    testvecs[TESTVEC_SAWTOOTH_SMALL].size = 512;
    j = 0;
    for (i = 0; i < 512; i++) {
        testvecs[TESTVEC_SAWTOOTH_SMALL].data[i] = j;
        j += 128;
    }

    // Test vector 3: Medium sawtooth: Increases by 16
    sprintf(testvecs[TESTVEC_SAWTOOTH_MEDIUM].name, "SAWTOOTH_MEDIUM");
    testvecs[TESTVEC_SAWTOOTH_MEDIUM].size = 512;
    j = 0;
    for (i = 0; i < 512; i++) {
        testvecs[TESTVEC_SAWTOOTH_MEDIUM].data[i] = j;
        j += 16;
    }

    // Test vector 4: Large sawtooth: Increases by 4
    sprintf(testvecs[TESTVEC_SAWTOOTH_LARGE].name, "SAWTOOTH_LARGE");
    testvecs[TESTVEC_SAWTOOTH_LARGE].size = 512;
    j = 0;
    for (i = 0; i < 512; i++) {
        testvecs[TESTVEC_SAWTOOTH_LARGE].data[i] = j;
        j += 4;
    }

    // Test vector 5: All random
    sprintf(testvecs[TESTVEC_ALL_RANDOM].name, "ALL_RANDOM");
    testvecs[TESTVEC_ALL_RANDOM].size = 512;
    j = 0;
    for (i = 0; i < 512; i++) {
        testvecs[TESTVEC_ALL_RANDOM].data[i] = rand();
    }

    // Test vector 6: Random, zero, random, zero, random, ...
    sprintf(testvecs[TESTVEC_ALTERNATING_RANDOM_ZERO].name, "ALTERNATING_RANDOM_ZERO");
    testvecs[TESTVEC_ALTERNATING_RANDOM_ZERO].size = 512;
    j = 0;
    memset(testvecs[TESTVEC_ALTERNATING_RANDOM_ZERO].data, 0, 512);
    for (i = 0; i < 512; i++) {
        testvecs[TESTVEC_ALTERNATING_RANDOM_ZERO].data[i] = rand();
        i++;
    }

    // Test vector 7: 8 times random, 8 times zero, 8 times random, ...
    sprintf(testvecs[TESTVEC_BLOCKS_8RANDOM_8ZERO].name, "BLOCKS_8RANDOM_8ZERO");
    testvecs[TESTVEC_BLOCKS_8RANDOM_8ZERO].size = 512;
    j = 0;
    memset(testvecs[TESTVEC_BLOCKS_8RANDOM_8ZERO].data, 0, 512);
    for (i = 0; i < 512; i++) {
        if (i%16 < 8) {
            testvecs[TESTVEC_BLOCKS_8RANDOM_8ZERO].data[i] = rand();
        }
    }

    finish_time = time_us_32();
    elapsed_time = finish_time - start_time;
    printf("%luµs\n", elapsed_time);

    printf("%d test vectors:\n", TESTVEC_NUM_TOTAL);
    for (i = 0; i < TESTVEC_NUM_TOTAL; i++) {
        printf("\t%s\n", testvecs[i].name);
    }
    printf("\n");

    ////////////////////////////////////////////////////
    // INIT ALGORITHMS /////////////////////////////////
    ////////////////////////////////////////////////////
    printf("Initializing compression algorithms ... ");
    start_time = time_us_32();

    sprintf(algos[ALGO_HEATSHRINK].name, "HEATSHRINK");
    algos[ALGO_HEATSHRINK].init = heatshrink_init;
    algos[ALGO_HEATSHRINK].compress = heatshrink_compress;
    algos[ALGO_HEATSHRINK].uncompress = heatshrink_uncompress;
    algos[ALGO_HEATSHRINK].init();

    sprintf(algos[ALGO_ZLIB].name, "ZLIB");
    algos[ALGO_ZLIB].init = zlib_init;
    algos[ALGO_ZLIB].compress = zlib_compress;
    algos[ALGO_ZLIB].uncompress = zlib_uncompress;
    algos[ALGO_ZLIB].init();

#if HAVE_ALGO_ZSTD
    sprintf(algos[ALGO_ZSTD].name, "ZSTD");
    algos[ALGO_ZSTD].init = zstd_init;
    algos[ALGO_ZSTD].compress = zstd_compress;
    algos[ALGO_ZSTD].uncompress = zstd_uncompress;
    algos[ALGO_ZSTD].init();
#endif

    sprintf(algos[ALGO_SNAPPY].name, "SNAPPY");
    algos[ALGO_SNAPPY].init = mysnappy_init;
    algos[ALGO_SNAPPY].compress = mysnappy_compress;
    algos[ALGO_SNAPPY].uncompress = mysnappy_uncompress;
    algos[ALGO_SNAPPY].init();

    sprintf(algos[ALGO_SNAPPYC].name, "SNAPPY-C");
    algos[ALGO_SNAPPYC].init = mysnappyc_init;
    algos[ALGO_SNAPPYC].compress = mysnappyc_compress;
    algos[ALGO_SNAPPYC].uncompress = mysnappyc_uncompress;
    algos[ALGO_SNAPPYC].init();

#if HAVE_ALGO_BROTLI
    sprintf(algos[ALGO_BROTLI].name, "BROTLI");
    algos[ALGO_BROTLI].init = brotli_init;
    algos[ALGO_BROTLI].compress = brotli_compress;
    algos[ALGO_BROTLI].uncompress = brotli_uncompress;
    algos[ALGO_BROTLI].init();
#endif

    finish_time = time_us_32();
    elapsed_time = finish_time - start_time;
    printf("%luµs\n", elapsed_time);

    printf("%d compression algorithms:\n", ALGO_NUM_TOTAL);
    for (i = 0; i < ALGO_NUM_TOTAL; i++) {
        printf("\t%s\n", algos[i].name);
    }
    printf("\n");

    ////////////////////////////////////////////////////
    // Run the benchmark ///////////////////////////////
    ////////////////////////////////////////////////////
    printf("BENCHMARK RUN START\n\n");
    for (i = 0; i < ALGO_NUM_TOTAL; i++) {
        printf("%s:\n", algos[i].name);
        for (j = 0; j < TESTVEC_NUM_TOTAL; j++) {
            printf("\t%s:", testvecs[j].name);
            size_t outsize = OUTBUF_SIZE;
            size_t outsize_2 = OUTBUF_SIZE;

            // Compress
            memset(outbuf, 0, OUTBUF_SIZE);
            start_time = time_us_32();
            algos[i].compress(testvecs[j].data, testvecs[j].size, outbuf, &outsize);
            finish_time = time_us_32();
            elapsed_time = finish_time - start_time;

            // Uncompress for verification and timing
            memset(outbuf_2, 0, OUTBUF_SIZE);
            start_time = time_us_32();
            algos[i].uncompress(outbuf, outsize, outbuf_2, &outsize_2);
            finish_time = time_us_32();
            elapsed_time_2 = finish_time - start_time;

            int ratio = outsize * 100 / testvecs[j].size;
            for(int k = 0; k < (32 - strlen(testvecs[j].name)); k++) {
                printf(" ");
            }
            printf("Runtime: %5luµs;\tInsize: %3u;\tOutsize: %3u,\tRatio: %3d%%\tDecompress time: %5luµs\tVerification: %s\n",
                elapsed_time, testvecs[j].size, outsize, ratio, elapsed_time_2, memcmp(testvecs[j].data, outbuf_2, testvecs[j].size) ? "FAILED" : "PASSED");
        }
    }

    printf("\n");
    sleep_ms(500);
    printf("\n");
    sleep_ms(500);
    printf("\n");
};
