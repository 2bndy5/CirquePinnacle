# auto-generate utility driver sources and/or linked libs
set(CIRQUE_PINNACLE_UTILITY_HEADERS
    ${CMAKE_CURRENT_LIST_DIR}/${PINNACLE_DRIVER}/gpio.h
    ${CMAKE_CURRENT_LIST_DIR}/${PINNACLE_DRIVER}/spi.h
    ${CMAKE_CURRENT_LIST_DIR}/${PINNACLE_DRIVER}/i2c.h
    ${CMAKE_CURRENT_LIST_DIR}/${PINNACLE_DRIVER}/time_keeping.h
)
set(PINNACLE_UTILITY_IMPLEMENTATION_FILES
    ${CMAKE_CURRENT_LIST_DIR}/${PINNACLE_DRIVER}/spi.cpp
    ${CMAKE_CURRENT_LIST_DIR}/${PINNACLE_DRIVER}/i2c.cpp
    ${CMAKE_CURRENT_LIST_DIR}/${PINNACLE_DRIVER}/time_keeping.cpp
)
if("${PINNACLE_DRIVER}" STREQUAL "bcm2xxx") # use bcm2835 lib
    install(FILES
            ${PINNACLE_DRIVER}/bcm2835.h
            ${CIRQUE_PINNACLE_UTILITY_HEADERS}
        DESTINATION include/CirquePinnacle/utility/${PINNACLE_DRIVER}
    )
    set(PINNACLE_DRIVER_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/${PINNACLE_DRIVER}/bcm2835.c
        ${PINNACLE_UTILITY_IMPLEMENTATION_FILES}
        PARENT_SCOPE
    )
elseif("${PINNACLE_DRIVER}" STREQUAL "linux_kernel") # use linux_kernel
    if(NOT SPIDEV_EXISTS)
        message(WARNING "Detecting /dev/spidev0.0 failed - continuing anyway. Please make sure SPI is enabled.")
    endif()
    install(FILES
            ${CIRQUE_PINNACLE_UTILITY_HEADERS}
        DESTINATION include/CirquePinnacle/utility/${PINNACLE_DRIVER}
    )
    set(PINNACLE_DRIVER_SOURCES
        ${PINNACLE_UTILITY_IMPLEMENTATION_FILES}
        ${CMAKE_CURRENT_LIST_DIR}/${PINNACLE_DRIVER}/gpio.cpp
        PARENT_SCOPE
    )
elseif("${PINNACLE_DRIVER}" STREQUAL "mraa") # use mraa
    install(FILES
            ${CIRQUE_PINNACLE_UTILITY_HEADERS}
        DESTINATION include/CirquePinnacle/utility/${PINNACLE_DRIVER}
    )
    set(CIRQUE_PINNACLE_LINKED_DRIVER ${LibMRAA} PARENT_SCOPE)
    set(PINNACLE_DRIVER_SOURCES
        ${PINNACLE_UTILITY_IMPLEMENTATION_FILES}
        ${CMAKE_CURRENT_LIST_DIR}/${PINNACLE_DRIVER}/gpio.cpp
        PARENT_SCOPE
    )
elseif("${PINNACLE_DRIVER}" STREQUAL "pigpio") # use pigpio
    install(FILES
            ${CIRQUE_PINNACLE_UTILITY_HEADERS}
        DESTINATION include/CirquePinnacle/utility/${PINNACLE_DRIVER}
    )
    set(CIRQUE_PINNACLE_LINKED_DRIVER ${LibPIGPIO} PARENT_SCOPE)
    set(PINNACLE_DRIVER_SOURCES
        ${PINNACLE_UTILITY_IMPLEMENTATION_FILES}
        ${CMAKE_CURRENT_LIST_DIR}/${PINNACLE_DRIVER}/gpio.cpp
        PARENT_SCOPE
    )
else() # No valid/supported driver selected nor detected... this is vital
    message(FATAL_ERROR "No valid/supported driver selected or auto-detection failed to resolve one.
        Please specify 1 of the following supported drivers (ie `-D PINNACLE_DRIVER=linux_kernel`):
        \tlinux_kernel
        \tbcm2xxx
        \tpigpio
        \tMRAA"
    )
endif()

configure_file(${CMAKE_CURRENT_LIST_DIR}/includes.h.in ${CMAKE_CURRENT_LIST_DIR}/includes.h)
