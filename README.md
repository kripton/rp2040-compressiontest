# rp2040-compressiontest: Benchmark of different compression algorithms on the rp2040

## Why I did this

I am currently coding an open-source multi-universe USB-DMX512-interface 
(https://github.com/OpenLightingProject/rp2040-dongle). DMX512 has the advantage 
that unused channels (and there are usually plenty of them just have the value 
of 0. That makes the amount of data shrink by a large amount if compressed.

Of course, the rp2040 is an "embedded" processor, so it's not the fastest 
thing to run code and memory is also pretty limited. So what to do if there are 
plenty compression algorithms out there to choose from? Benchmark them :)

Please be aware that my use case is pretty limited and constrained: I'm "only" 
compressing 512 byte each and I want to do so FAST (no longer than 2ms). Your 
use case and requirements might be hugely different.

## For the impatient: Here are my results (CPU overclocked to 250MHz)
```
Welcome to rp2040-compressiontest
pll_sys  = 250001kHz (= ~250MHz)

Initializing test vectors ... 495µs
8 test vectors:
        ALL_ZERO
        ALL_ONE
        SAWTOOTH_SMALL
        SAWTOOTH_MEDIUM
        SAWTOOTH_LARGE
        ALL_RANDOM
        ALTERNATING_RANDOM_ZERO
        BLOCKS_8RANDOM_8ZERO

Initializing compression algorithms ... 95µs
5 compression algorithms:
        HEATSHRINK
        ZLIB
        ZSTD
        SNAPPY
        BROTLI

BENCHMARK RUN START

HEATSHRINK:
        ALL_ZERO:                        Runtime:   268µs;      Insize: 512;    Outsize:  16,   Ratio:   3%     Decompress time:   151µs        Verification: PASSED
        ALL_ONE:                         Runtime:   236µs;      Insize: 512;    Outsize:  18,   Ratio:   3%     Decompress time:   131µs        Verification: PASSED
        SAWTOOTH_SMALL:                  Runtime:   257µs;      Insize: 512;    Outsize:  19,   Ratio:   3%     Decompress time:   123µs        Verification: PASSED
        SAWTOOTH_MEDIUM:                 Runtime:   281µs;      Insize: 512;    Outsize:  34,   Ratio:   6%     Decompress time:   141µs        Verification: PASSED
        SAWTOOTH_LARGE:                  Runtime:   382µs;      Insize: 512;    Outsize:  86,   Ratio:  16%     Decompress time:   205µs        Verification: PASSED
        ALL_RANDOM:                      Runtime:  1193µs;      Insize: 512;    Outsize: 576,   Ratio: 112%     Decompress time:   824µs        Verification: PASSED
        ALTERNATING_RANDOM_ZERO:         Runtime:  8384µs;      Insize: 512;    Outsize: 479,   Ratio:  93%     Decompress time:   747µs        Verification: PASSED
        BLOCKS_8RANDOM_8ZERO:            Runtime:  2109µs;      Insize: 512;    Outsize: 348,   Ratio:  67%     Decompress time:   563µs        Verification: PASSED
ZLIB:
        ALL_ZERO:                        Runtime:   670µs;      Insize: 512;    Outsize:  14,   Ratio:   2%     Decompress time:   166µs        Verification: PASSED
        ALL_ONE:                         Runtime:   537µs;      Insize: 512;    Outsize:  14,   Ratio:   2%     Decompress time:   100µs        Verification: PASSED
        SAWTOOTH_SMALL:                  Runtime:   514µs;      Insize: 512;    Outsize:  15,   Ratio:   2%     Decompress time:    94µs        Verification: PASSED
        SAWTOOTH_MEDIUM:                 Runtime:   569µs;      Insize: 512;    Outsize:  30,   Ratio:   5%     Decompress time:   103µs        Verification: PASSED
        SAWTOOTH_LARGE:                  Runtime:   806µs;      Insize: 512;    Outsize:  81,   Ratio:  15%     Decompress time:   130µs        Verification: PASSED
        ALL_RANDOM:                      Runtime:  2042µs;      Insize: 512;    Outsize: 523,   Ratio: 102%     Decompress time:    86µs        Verification: PASSED
        ALTERNATING_RANDOM_ZERO:         Runtime:  1994µs;      Insize: 512;    Outsize: 393,   Ratio:  76%     Decompress time:   501µs        Verification: PASSED
        BLOCKS_8RANDOM_8ZERO:            Runtime:  1877µs;      Insize: 512;    Outsize: 331,   Ratio:  64%     Decompress time:   227µs        Verification: PASSED
ZSTD:
        ALL_ZERO:                        Runtime:   610µs;      Insize: 512;    Outsize:  19,   Ratio:   3%     Decompress time:   307µs        Verification: PASSED
        ALL_ONE:                         Runtime:   508µs;      Insize: 512;    Outsize:  19,   Ratio:   3%     Decompress time:   139µs        Verification: PASSED
        SAWTOOTH_SMALL:                  Runtime:   472µs;      Insize: 512;    Outsize:  19,   Ratio:   3%     Decompress time:   130µs        Verification: PASSED
        SAWTOOTH_MEDIUM:                 Runtime:   466µs;      Insize: 512;    Outsize:  33,   Ratio:   6%     Decompress time:   117µs        Verification: PASSED
        SAWTOOTH_LARGE:                  Runtime:   543µs;      Insize: 512;    Outsize:  83,   Ratio:  16%     Decompress time:   128µs        Verification: PASSED
        ALL_RANDOM:                      Runtime:   578µs;      Insize: 512;    Outsize: 522,   Ratio: 101%     Decompress time:    63µs        Verification: PASSED
        ALTERNATING_RANDOM_ZERO:         Runtime:  1422µs;      Insize: 512;    Outsize: 379,   Ratio:  74%     Decompress time:   654µs        Verification: PASSED
        BLOCKS_8RANDOM_8ZERO:            Runtime:  1822µs;      Insize: 512;    Outsize: 345,   Ratio:  67%     Decompress time:   235µs        Verification: PASSED
SNAPPY:
        ALL_ZERO:                        Runtime:   191µs;      Insize: 512;    Outsize:  28,   Ratio:   5%     Decompress time:   235µs        Verification: PASSED
        ALL_ONE:                         Runtime:   128µs;      Insize: 512;    Outsize:  28,   Ratio:   5%     Decompress time:   103µs        Verification: PASSED
        SAWTOOTH_SMALL:                  Runtime:   122µs;      Insize: 512;    Outsize:  29,   Ratio:   5%     Decompress time:    99µs        Verification: PASSED
        SAWTOOTH_MEDIUM:                 Runtime:   120µs;      Insize: 512;    Outsize:  43,   Ratio:   8%     Decompress time:    75µs        Verification: PASSED
        SAWTOOTH_LARGE:                  Runtime:   133µs;      Insize: 512;    Outsize:  90,   Ratio:  17%     Decompress time:    58µs        Verification: PASSED
        ALL_RANDOM:                      Runtime:    74µs;      Insize: 512;    Outsize: 517,   Ratio: 100%     Decompress time:    46µs        Verification: PASSED
        ALTERNATING_RANDOM_ZERO:         Runtime:    76µs;      Insize: 512;    Outsize: 517,   Ratio: 100%     Decompress time:    34µs        Verification: PASSED
        BLOCKS_8RANDOM_8ZERO:            Runtime:   249µs;      Insize: 512;    Outsize: 359,   Ratio:  70%     Decompress time:   243µs        Verification: PASSED
BROTLI:
        ALL_ZERO:                        Runtime:   733µs;      Insize: 512;    Outsize:  20,   Ratio:   3%     Decompress time:   568µs        Verification: PASSED
        ALL_ONE:                         Runtime:   594µs;      Insize: 512;    Outsize:  20,   Ratio:   3%     Decompress time:   536µs        Verification: PASSED
        SAWTOOTH_SMALL:                  Runtime:   679µs;      Insize: 512;    Outsize:  21,   Ratio:   4%     Decompress time:   408µs        Verification: PASSED
        SAWTOOTH_MEDIUM:                 Runtime:   841µs;      Insize: 512;    Outsize:  67,   Ratio:  13%     Decompress time:   438µs        Verification: PASSED
        SAWTOOTH_LARGE:                  Runtime:  1086µs;      Insize: 512;    Outsize: 162,   Ratio:  31%     Decompress time:   531µs        Verification: PASSED
        ALL_RANDOM:                      Runtime:  2412µs;      Insize: 512;    Outsize: 516,   Ratio: 100%     Decompress time:   134µs        Verification: PASSED
        ALTERNATING_RANDOM_ZERO:         Runtime:  2168µs;      Insize: 512;    Outsize: 427,   Ratio:  83%     Decompress time:   827µs        Verification: PASSED
        BLOCKS_8RANDOM_8ZERO:            Runtime:  2027µs;      Insize: 512;    Outsize: 410,   Ratio:  80%     Decompress time:   881µs        Verification: PASSED
```


## Building from source

This project uses the usual toolchain for the RP2040-based projects. You will 
need a (cross-)compiler for the ARM Cortex M0 and the pico-sdk. For details 
how you can achieve this on your system, please refer to
https://datasheets.raspberrypi.org/pico/getting-started-with-pico.pdf

Basically, once all requirements are met, run:
```
mkdir build
cd build
cmake ../src
make
```
This should result in a file called `rp2040-compressiontest.uf2` that can be copied to the Pico when it's in bootloader-mode.

## License

This source code, schematics and board layouts is licensed under the GPL.

Parts taken and modified from TinyUSB (`tusb_config.h`, `usb_descriptors.c`) is:

     * The MIT License (MIT)
     * Copyright (c) 2019 Ha Thach (tinyusb.org)
