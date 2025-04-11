cmake_minimum_required(VERSION 3.12)

# Pull in SDK (must be before project)
include(../../src/cmake/pico_sdk_import.cmake)

project(pico_examples C CXX ASM)

# Initialize the Pico SDK
pico_sdk_init()

# In YOUR project, include CirquePinnacle lib's root-level CMakeLists.txt
# giving the path depending on where the library
# is cloned to in your project
include(../../src/CMakeLists.txt)

# These options are really useful in testing different interfaces.
# Typical applications shouldn't need them
option(USE_I2C "option to use a I2C bus instead of a SPI bus" OFF)

# iterate over a list of examples by name
set(EXAMPLES_LIST
    absolute_mode
    relative_mode
    anymeas_mode
)

if (USE_I2C)
    message(STATUS "Using I2C bus interface")
else()
    message(STATUS "Using SPI bus interface")
endif()

foreach(example ${EXAMPLES_LIST})
    # make a target
    add_executable(${example} ${example}.cpp defaultPins.h)

    # link the necessary libs to the target
    target_link_libraries(${example} PUBLIC
        CirquePinnacle
        pico_stdlib
        hardware_gpio
        hardware_spi
        hardware_i2c
    )

    if (USE_I2C)
        target_compile_definitions(${example} PUBLIC USE_I2C=1)
    endif()

    # specify USB port as default serial communication's interface (not UART RX/TX pins)
    pico_enable_stdio_usb(${example} 1)
    pico_enable_stdio_uart(${example} 0)

    # create map/bin/hex file etc.
    pico_add_extra_outputs(${example})
endforeach()

add_subdirectory(usb_mouse)
