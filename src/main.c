/**
 * SPDX-License-Identifier: GPL-3.0
 */

#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "pico/time.h"
#include "tusb.h"

#include "picotool_binary_information.h"


// TEST VECTORS
struct testVec {
    uint8_t           name[32];    // Human-readable name
    uint8_t           data[512];
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
};

enum {
    ALGO_HEATSHRINK,
    // Add more HERE
    ALGO_NUM_TOTAL
};

// Variables used for elapsed time calculation
uint32_t start_time, finish_time, elapsed_time;

uint16_t i;
uint8_t j;

int main() {
    // Overclock the board to 250MHz. According to
    // https://www.youtube.com/watch?v=G2BuoFNLo this should be
    // totally safe with the default 1.10V Vcore
    //set_sys_clock_khz(250000, true);

    // Enable the USB console
    stdio_usb_init();

    while (!tud_cdc_connected()) {
        sleep_ms(100);
    }

    printf("Welcome to rp2040-compressiontest\n\n");


    ////////////////////////////////////////////////////
    // INIT TEST VECTORS ///////////////////////////////
    ////////////////////////////////////////////////////
    printf("Initializing test vectors ... ");
    start_time = time_us_32();

    // Test vector 0: All zeroes
    sprintf(testvecs[TESTVEC_ALL_ZERO].name, "ALL_ZERO");
    memset(testvecs[TESTVEC_ALL_ZERO].data, 0, 512);

    // Test vector 1: All ones
    sprintf(testvecs[TESTVEC_ALL_ONE].name, "TESTVEC_ALL_ONE");
    memset(testvecs[TESTVEC_ALL_ONE].data, 0xff, 512);

    // Test vector 2: Very small sawtooth: 0, 127, 255, 0, ....
    sprintf(testvecs[TESTVEC_SAWTOOTH_SMALL].name, "SAWTOOTH_SMALL");
    j = 0;
    for (i = 0; i < 512; i++) {
        testvecs[TESTVEC_SAWTOOTH_SMALL].data[i] = j;
        j += 128;
    }

    // Test vector 3: Medium sawtooth: Increases by 16
    sprintf(testvecs[TESTVEC_SAWTOOTH_MEDIUM].name, "SAWTOOTH_MEDIUM");
    j = 0;
    for (i = 0; i < 512; i++) {
        testvecs[TESTVEC_SAWTOOTH_MEDIUM].data[i] = j;
        j += 16;
    }

    // Test vector 4: Large sawtooth: Increases by 4
    sprintf(testvecs[TESTVEC_SAWTOOTH_LARGE].name, "SAWTOOTH_LARGE");
    j = 0;
    for (i = 0; i < 512; i++) {
        testvecs[TESTVEC_SAWTOOTH_LARGE].data[i] = j;
        j += 4;
    }

    // Test vector 5: All random
    sprintf(testvecs[TESTVEC_ALL_RANDOM].name, "ALL_RANDOM");
    j = 0;
    for (i = 0; i < 512; i++) {
        testvecs[TESTVEC_ALL_RANDOM].data[i] = rand();
    }

    // Test vector 6: Random, zero, random, zero, random, ...
    sprintf(testvecs[TESTVEC_ALTERNATING_RANDOM_ZERO].name, "ALTERNATING_RANDOM_ZERO");
    j = 0;
    memset(testvecs[TESTVEC_ALTERNATING_RANDOM_ZERO].data, 0, 512);
    for (i = 0; i < 512; i++) {
        testvecs[TESTVEC_ALTERNATING_RANDOM_ZERO].data[i] = rand();
        i++;
    }

    // Test vector 7: 8 times random, 8 times zero, 8 times random, ...
    sprintf(testvecs[TESTVEC_BLOCKS_8RANDOM_8ZERO].name, "BLOCKS_8RANDOM_8ZERO");
    j = 0;
    memset(testvecs[TESTVEC_BLOCKS_8RANDOM_8ZERO].data, 0, 512);
    for (i = 0; i < 512; i++) {
        if (i%16 < 8) {
            testvecs[TESTVEC_BLOCKS_8RANDOM_8ZERO].data[i] = rand();
        }
    }

    finish_time = time_us_32();
    elapsed_time = finish_time - start_time;
    printf("%lu us\n", elapsed_time);

    ////////////////////////////////////////////////////
    // INIT ALGORITHMS /////////////////////////////////
    ////////////////////////////////////////////////////

};
