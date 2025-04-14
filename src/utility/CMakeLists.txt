# auto-generate utility driver sources and/or linked libs
if("${PINNACLE_DRIVER}" STREQUAL "linux_kernel") # use linux_kernel
    if(NOT SPIDEV_EXISTS)
        message(WARNING "Detecting /dev/spidev0.0 failed - continuing anyway. Please make sure SPI is enabled.")
    endif()
    install(FILES
            ${CMAKE_CURRENT_LIST_DIR}/${PINNACLE_DRIVER}/gpio.h
            ${CMAKE_CURRENT_LIST_DIR}/${PINNACLE_DRIVER}/interrupt.h
            ${CMAKE_CURRENT_LIST_DIR}/${PINNACLE_DRIVER}/spi.h
            ${CMAKE_CURRENT_LIST_DIR}/${PINNACLE_DRIVER}/i2c.h
            ${CMAKE_CURRENT_LIST_DIR}/${PINNACLE_DRIVER}/time_keeping.h
        DESTINATION include/CirquePinnacle/utility/${PINNACLE_DRIVER}
    )
    set(PINNACLE_DRIVER_SOURCES
            ${CMAKE_CURRENT_LIST_DIR}/${PINNACLE_DRIVER}/spi.cpp
            ${CMAKE_CURRENT_LIST_DIR}/${PINNACLE_DRIVER}/i2c.cpp
            ${CMAKE_CURRENT_LIST_DIR}/${PINNACLE_DRIVER}/time_keeping.cpp
            ${CMAKE_CURRENT_LIST_DIR}/${PINNACLE_DRIVER}/gpio.cpp
            ${CMAKE_CURRENT_LIST_DIR}/${PINNACLE_DRIVER}/interrupt.cpp
        PARENT_SCOPE
    )
else() # No valid/supported driver selected nor detected... this is vital
    message(FATAL_ERROR "No valid/supported driver selected.
        Please specify 1 of the following supported drivers (ie `-D PINNACLE_DRIVER=linux_kernel`):
        \tlinux_kernel"
    )
endif()

configure_file(${CMAKE_CURRENT_LIST_DIR}/includes.h.in ${CMAKE_CURRENT_LIST_DIR}/includes.h)
