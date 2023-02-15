set(CIRQUE_PINNACLE_LINKED_DRIVER "")
set(CIRQUE_PINNACLE_DRIVER "UNKNOWN" CACHE STRING "override automatic configuration of CirquePinnacle's utility driver.
    Specify 1 of the following supported drivers (ie -DCIRQUE_PINNACLE_DRIVER=linux_kernel):
    linux_kernel
    bcm2xxx
    pigpio
    mraa"
)

###########################
# detect pre-existing (locally installed) 3rd party libraries
###########################

# detect installed libraries despite what CIRQUE_PINNACLE_DRIVER is set to
# this is always done for cross-compiling purposes
find_library(LibMRAA mraa)
find_library(LibPIGPIO pigpio)
if(EXISTS /dev/spidev0.0)
    set(SPIDEV_EXISTS TRUE)
else()
    set(SPIDEV_EXISTS FALSE)
endif()


if(${CIRQUE_PINNACLE_DRIVER} STREQUAL "UNKNOWN") # invokes automatic configuration
    if("${SOC}" STREQUAL "BCM2708" OR "${SOC}" STREQUAL "BCM2709" OR "${SOC}" STREQUAL "BCM2835")
        set(CIRQUE_PINNACLE_DRIVER bcm2xxx CACHE STRING "using folder /utility/bcm2xxx" FORCE)
    elseif(NOT "${LibPIGPIO}" STREQUAL "LibPIGPIO-NOTFOUND")
        message(STATUS "Found pigpio library: ${LibPIGPIO}")
        set(CIRQUE_PINNACLE_DRIVER pigpio CACHE STRING "using folder /utility/pigpio" FORCE)
    elseif(NOT "${LibMRAA}" STREQUAL "LibMRAA-NOTFOUND")
        message(STATUS "Found mraa library: ${LibMRAA}")
        set(CIRQUE_PINNACLE_DRIVER mraa CACHE STRING "using folder /utility/mraa" FORCE)
    elseif(SPIDEV_EXISTS) # should be a non-empty string if SPI is enabled
        message(STATUS "detected that SPIDEV is enabled: ${SPIDEV_EXISTS}")
        set(CIRQUE_PINNACLE_DRIVER linux_kernel CACHE STRING "using folder /utility/linux_kernel" FORCE)
    endif()
endif()

# override the auto-detect if CIRQUE_PINNACLE_DRIVER is defined in an env var
if(DEFINED ENV{CIRQUE_PINNACLE_DRIVER})
    message(STATUS "CIRQUE_PINNACLE_DRIVER (set from env var) = $ENV{CIRQUE_PINNACLE_DRIVER}")
    set(CIRQUE_PINNACLE_DRIVER $ENV{CIRQUE_PINNACLE_DRIVER} CACHE STRING "" FORCE)
endif()

message(STATUS "Using driver: ${CIRQUE_PINNACLE_DRIVER}")
