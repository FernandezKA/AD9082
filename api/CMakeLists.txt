cmake_minimum_required(VERSION 3.16)

# AD9081 API Library
add_library(ad9081_api
    ad9081.c
    api/adi_ad9081_adc.c
    api/adi_ad9081_dac.c
    api/adi_ad9081_device.c
    api/adi_ad9081_hal.c
    api/adi_ad9081_jesd.c
    api/adi_ad9081_sync.c
)

target_include_directories(ad9081_api PUBLIC
    .
    api
    ./inc
)

target_compile_definitions(ad9081_api PRIVATE
    __STDC_FORMAT_MACROS
)