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

# iterate over a list of examples by name
set(EXAMPLES_LIST
    absolute_mode
    relative_mode
    anymeas_mode
)

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

    # specify USB port as default serial communication's interface (not UART RX/TX pins)
    pico_enable_stdio_usb(${example} 1)
    pico_enable_stdio_uart(${example} 0)

    # create map/bin/hex file etc.
    pico_add_extra_outputs(${example})
endforeach()
