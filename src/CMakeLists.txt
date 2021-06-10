cmake_minimum_required(VERSION 3.13)

## pico_sdk_import.cmake is copied from
## https://github.com/raspberrypi/pico-sdk/blob/master/external/pico_sdk_import.cmake
include(pico_sdk_import.cmake)

## Give the baby a name. Will also be the name of the resulting binaries
project(rp2040-compressiontest)

## Init the pico-sdk. Will instruct the user what to do if deps are missing
pico_sdk_init()

## Include the CMakeLists-files of (non-pico-sdk)-libraries we will be using
include(../lib/heatshrink/CMakeLists.txt)
include(../lib/zlib/interfaceLibForPicoSDK.cmake)
include(../lib/zstd/interfaceLibForPicoSDK.cmake)
include(../lib/snappy/interfaceLibForPicoSDK.cmake)

## Add our own C/C++ files here
## Sorted alphabetically
add_executable(${CMAKE_PROJECT_NAME}
    ${TINYUSB_LIBNETWORKING_SOURCES}
    algo-heatshrink.c
    algo-zlib.c
    algo-zstd.c
    algo-snappy.c
    main.c
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
)

## enable usb output, disable uart output
pico_enable_stdio_usb(${CMAKE_PROJECT_NAME} 1)
pico_enable_stdio_uart(${CMAKE_PROJECT_NAME} 0)

## Create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(${CMAKE_PROJECT_NAME})
