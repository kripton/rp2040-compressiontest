# rp2040-compressiontest: Benchmark of different compression algorithms on the rp2040

### Building from source

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
