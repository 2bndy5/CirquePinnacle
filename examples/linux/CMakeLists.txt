cmake_minimum_required(VERSION 3.12)

project(CirquePinnacleExamples CXX)
add_compile_options(-Ofast -Wall) # passing the compiler a `-pthread` flag doesn't work here

# detect the CPU make and type
include(../../src/cmake/detectCPU.cmake) # sets the variable SOC accordingly

# auto-detect what driver to use
# auto-detect can be overriden using `cmake .. -D CIRQUE_PINNACLE_DRIVER=<supported driver>`
include(../../src/cmake/auto_config_driver.cmake)

find_library(CirquePinnacle cirquepinnacle REQUIRED)
message(STATUS "using CirquePinnacle library: ${CirquePinnacle}")

# iterate over a list of examples by name
set(EXAMPLES_LIST
    absolute_mode
    relative_mode
    anymeas_mode
)

foreach(example ${EXAMPLES_LIST})
    # make a target
    add_executable(${example} ${example}.cpp)

    # link the CirquePinnacle lib to the target. Notice we specify pthread as a linked lib here
    if("${CIRQUE_PINNACLE_DRIVER}" STREQUAL "mraa")
        target_link_libraries(${example} PUBLIC ${CirquePinnacle} pthread ${LibMRAA})
    elseif("${CIRQUE_PINNACLE_DRIVER}" STREQUAL "pigpio")
        target_link_libraries(${example} PUBLIC ${CirquePinnacle} ${LibPIGPIO} pthread)
    else() # not using mraa or pigpio drivers
        target_link_libraries(${example} PUBLIC ${CirquePinnacle} pthread)
    endif()

    if(DEFINED PINNACLE_NO_ANYMEAS_SUPPORT)
        message(STATUS "Excluding code related to the Pinnacle's anymeas mode.")
        target_compile_definitions(${example} PUBLIC PINNACLE_NO_ANYMEAS_SUPPORT=1)
    endif()

endforeach()
