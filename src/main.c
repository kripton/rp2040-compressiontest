/**
 * SPDX-License-Identifier: GPL-3.0
 */

#include <stdio.h>
#include "pico/stdlib.h"

#include "picotool_binary_information.h"

int main() {
    // Overclock the board to 250MHz. According to
    // https://www.youtube.com/watch?v=G2BuoFNLo this should be
    // totally safe with the default 1.10V Vcore
    //set_sys_clock_khz(250000, true);

    // Enable the USB console
    stdio_usb_init();

};
