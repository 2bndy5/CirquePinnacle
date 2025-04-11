cmake_minimum_required(VERSION 3.12)

project(CirquePinnacleExamples CXX)
add_compile_options(-Ofast -Wall) # passing the compiler a `-pthread` flag doesn't work here

# detect the CPU make and type
include(../../src/cmake/detectCPU.cmake) # sets the variable SOC accordingly

# auto-detect what driver to use
include(../../src/cmake/auto_config_driver.cmake)

find_library(CirquePinnacle cirquepinnacle REQUIRED)
message(STATUS "using CirquePinnacle library: ${CirquePinnacle}")

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
    add_executable(${example} ${example}.cpp)

    # link the CirquePinnacle lib to the target. Notice we specify pthread as a linked lib here
    target_link_libraries(${example} PUBLIC ${CirquePinnacle} pthread)

    if (USE_I2C)
        target_compile_definitions(${example} PUBLIC USE_I2C=1)
    endif()
endforeach()
