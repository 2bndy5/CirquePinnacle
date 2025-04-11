set(PINNACLE_DRIVER "linux_kernel" CACHE STRING "using folder src/utility/linux_kernel")

# detect installed libraries despite what PINNACLE_DRIVER is set to
# this is always done for cross-compiling purposes
if(EXISTS /dev/spidev0.0)
    set(SPIDEV_EXISTS TRUE)
else()
    set(SPIDEV_EXISTS FALSE)
endif()


if(NOT SPIDEV_EXISTS)
    message(STATUS "detected that SPIDEV is enabled: ${SPIDEV_EXISTS}")
endif()

message(STATUS "Using driver: ${PINNACLE_DRIVER}")
