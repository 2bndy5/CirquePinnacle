cmake_minimum_required(VERSION 3.13)

set(targetName "dev_hid_composite")
add_executable(${targetName})

target_sources(${targetName} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/usb_descriptors.c
)

# Make sure TinyUSB can find tusb_config.h
target_include_directories(${targetName} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)

# In addition to pico_stdlib required for common PicoSDK functionality, add dependency on tinyusb_device
# for TinyUSB device support and tinyusb_board for the additional board support library used by the example
target_link_libraries(${targetName} PUBLIC
    pico_stdlib
    tinyusb_device
    tinyusb_board
    hardware_gpio
    hardware_spi
    hardware_i2c
    CirquePinnacle
)

if (USE_I2C)
    target_compile_definitions(${targetName} PUBLIC USE_I2C=1)
endif()

target_compile_definitions(${targetName} PUBLIC PICO_DEFAULT_I2C=1)

# Uncomment this line to enable fix for Errata RP2040-E5 (the fix requires use of GPIO 15)
# target_compile_definitions(${targetName} PUBLIC PICO_RP2040_USB_DEVICE_ENUMERATION_FIX=1)

pico_add_extra_outputs(dev_hid_composite)
