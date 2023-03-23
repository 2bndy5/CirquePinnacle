cmake_minimum_required(VERSION 3.12)

project(CirquePinnacleExamples CXX)
add_compile_options(-Ofast -Wall) # passing the compiler a `-pthread` flag doesn't work here

# detect the CPU make and type
include(../../src/cmake/detectCPU.cmake) # sets the variable SOC accordingly

# auto-detect what driver to use
# auto-detect can be overriden using `cmake .. -D PINNACLE_DRIVER=<supported driver>`
include(../../src/cmake/auto_config_driver.cmake)

find_library(CirquePinnacle cirquepinnacle REQUIRED)
message(STATUS "using CirquePinnacle library: ${CirquePinnacle}")

# These options are really useful in testing different interfaces.
# Typical applications shouldn't need them
option(USE_I2C "option to use a I2C bus instead of a SPI bus" OFF)
option(USE_SW_DR "option to use no physical DR_PIN (incompatible with anymeas mode)" OFF)

# iterate over a list of examples by name
set(EXAMPLES_LIST
    absolute_mode
    relative_mode
)

if (USE_SW_DR)
    message(STATUS "Using software DR flag (skipping anymeas_mode example)")
else()
    message(STATUS "Using physical DR pin")
    if (USE_I2C)
        message(STATUS "Using I2C bus interface")
    else()
        message(STATUS "Using SPI bus interface")
        list(APPEND EXAMPLES_LIST anymeas_mode)
    endif()
endif()

foreach(example ${EXAMPLES_LIST})
    # make a target
    add_executable(${example} ${example}.cpp)

    # link the CirquePinnacle lib to the target. Notice we specify pthread as a linked lib here
    if("${PINNACLE_DRIVER}" STREQUAL "mraa")
        target_link_libraries(${example} PUBLIC ${CirquePinnacle} pthread ${LibMRAA})
    elseif("${PINNACLE_DRIVER}" STREQUAL "pigpio")
        target_link_libraries(${example} PUBLIC ${CirquePinnacle} ${LibPIGPIO} pthread)
    else() # not using mraa or pigpio drivers
        target_link_libraries(${example} PUBLIC ${CirquePinnacle} pthread)
    endif()

    if (USE_I2C)
        target_compile_definitions(${example} PUBLIC USE_I2C=1)
    endif()

    if (USE_SW_DR)
        target_compile_definitions(${example} PUBLIC USE_SW_DR=1)
    endif()

    if(DEFINED PINNACLE_NO_ANYMEAS_SUPPORT)
        message(STATUS "Excluding code related to the Pinnacle's anymeas mode.")
        target_compile_definitions(${example} PUBLIC PINNACLE_NO_ANYMEAS_SUPPORT=1)
    endif()

endforeach()
