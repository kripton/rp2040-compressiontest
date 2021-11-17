cmake_minimum_required(VERSION 3.13)

## pico_sdk_import.cmake is copied from
## https://github.com/raspberrypi/pico-sdk/blob/master/external/pico_sdk_import.cmake
include(pico_sdk_import.cmake)

## Give the baby a name. Will also be the name of the resulting binaries
project(rp2040-compressiontest)

## Init the pico-sdk. Will instruct the user what to do if deps are missing
pico_sdk_init()

## Needed so we can fit all compression algos into the Picos flash
## See below. However, needs to be set here in order to be effective
add_compile_options(-Os)

## I'd like to use LTO but it doesn't yet work
## https://github.com/raspberrypi/pico-sdk/issues/97
#add_compile_options(-flto -fno-fat-lto-objects -flto-partition=1to1 -fuse-ld=bfd -v)
#add_compile_options(-flto)
#add_link_options(-Wl,-v -v -t -t)

## Include the CMakeLists-files of (non-pico-sdk)-libraries we will be using
include(../lib/heatshrink/CMakeLists.txt)
include(../lib/zlib/interfaceLibForPicoSDK.cmake)
include(../lib/zstd/interfaceLibForPicoSDK.cmake)
include(../lib/snappy/interfaceLibForPicoSDK.cmake)
include(../lib/brotli/interfaceLibForPicoSDK.cmake)

## Add our own C/C++ files here
## Sorted alphabetically
add_executable(${CMAKE_PROJECT_NAME}
    ${TINYUSB_LIBNETWORKING_SOURCES}
    algo-heatshrink.c
    algo-zlib.c
    algo-zstd.c
    algo-snappy.c
    algo-brotli.c
    main.c
)

## Unfortunately, including all algorithms would result in a binary file
## too large for the Pico board (~2.7MB vs. 2MB available)
## We could try -Os but that contradicts our benchmarking effort ... UPDATE:
## just trying it revealed that the code is not that much slower.
## Using -flto broke the build :/
## This is why you can either exclude ZSTD OR BROTLI, which both add a
## remarkable amount to the size of the UF2 file.
## When using -Os (see above), all algos just fit
target_compile_definitions(${CMAKE_PROJECT_NAME}
    PUBLIC HAVE_ALGO_ZSTD=1
    PUBLIC HAVE_ALGO_BROTLI=1
)

## Config for the heatshrink library
target_compile_definitions(${CMAKE_PROJECT_NAME}
    PUBLIC HEATSHRINK_DYNAMIC_ALLOC=0
    PUBLIC HEATSHRINK_STATIC_INPUT_BUFFER_SIZE=600
    PUBLIC HEATSHRINK_STATIC_WINDOW_BITS=9
    PUBLIC HEATSHRINK_STATIC_LOOKAHEAD_BITS=6
)

## Config for the zlib library to reduce memory usage
target_compile_definitions(${CMAKE_PROJECT_NAME}
    PUBLIC MAX_WBITS=9
    PUBLIC MAX_MEM_LEVEL=4
)

## Needed so tusb_config.h is found by the tinyUSB-library included in the pico-sdk
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)

## Pull in all pico-sdk + non-pico-sdk libraries
## Sorted alphabetically
target_link_libraries(${CMAKE_PROJECT_NAME}
    heatshrink
    pico_stdlib
    zlib
    zstd
    snappy
    brotli
)

## enable usb output, disable uart output
pico_enable_stdio_usb(${CMAKE_PROJECT_NAME} 1)
pico_enable_stdio_uart(${CMAKE_PROJECT_NAME} 0)

## Create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(${CMAKE_PROJECT_NAME})
