## Include this file if you want to use the CirquePinnacle library
## in YOUR (Pico) project.
## See examples_pico/CMakeLists.txt to see how it could be done

cmake_minimum_required(VERSION 3.12)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Define the CirquePinnacle core library
add_library(CirquePinnacle INTERFACE)

# generate the lib-specific includes.h for the Pico SDK
set(PINNACLE_DRIVER "rp2")
configure_file(${CMAKE_CURRENT_LIST_DIR}/../includes.h.in ${CMAKE_CURRENT_LIST_DIR}/../includes.h)

target_sources(CirquePinnacle INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/../../CirquePinnacle.cpp
    ${CMAKE_CURRENT_LIST_DIR}/gpio.cpp
    ${CMAKE_CURRENT_LIST_DIR}/spi.cpp
    ${CMAKE_CURRENT_LIST_DIR}/i2c.cpp
    ${CMAKE_CURRENT_LIST_DIR}/time_keeping.h
)

target_include_directories(CirquePinnacle INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/../../
)
