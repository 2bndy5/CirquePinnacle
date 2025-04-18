/*
 * Copyright (c) 2023 Brendan Doherty (2bndy5)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef CIRQUEPINNACLE_ARCH_COMMON_H_
#define CIRQUEPINNACLE_ARCH_COMMON_H_

#ifndef PINNACLE_ANYMEAS_SUPPORT
    /**
     * This will enable library support of the Pinnacle ASIC's anymeas mode (advanced usage).
     *
     * @note
     *     Arduino users need to comment out this line to save space on memory.
     *
     *     All other builds using CMake (including the python bindings) can simply define
     *     ``-D PINNACLE_ANYMEAS_SUPPORT=OFF`` as a CMake option.
     *
     * @ingroup cmake-options
     */
    #define PINNACLE_ANYMEAS_SUPPORT true
#endif // !defined(PINNACLE_ANYMEAS_SUPPORT)

#if defined(ARDUINO)
    #include <Arduino.h>
    #include <SPI.h>
    #include <Wire.h>

    #ifndef PINNACLE_SPI_SPEED
        // Default is the recommended 6 MHz (maximum supported SPI speed is 13 MHz)
        #define PINNACLE_SPI_SPEED 6000000
    #endif

/**
 * alias for SPI bus implementation
 *
 * @ingroup arduino-spi
 */
typedef SPIClass pinnacle_spi_t;

/**
 * alias for I2C bus implementation
 *
 * @ingroup arduino-i2c
 */
typedef TwoWire pinnacle_i2c_t;

    #ifdef NUM_DIGITAL_PINS
        #if NUM_DIGITAL_PINS < 0xFF
typedef uint8_t pinnacle_gpio_t;
        #else
typedef uint16_t pinnacle_gpio_t;
        #endif
    #else
typedef uint16_t pinnacle_gpio_t;
    #endif

    #define PINNACLE_SS_CTRL(pin, value) digitalWrite(pin, value)

    #define PINNACLE_USE_ARDUINO_API

#else
    // all other platforms/frameworks will be included via "utility/includes.h" file generated by CMake.
    // see src/cmake/auto_config_driver.cmake script for supported options (on linux)
    #include "utility/includes.h"
#endif

#endif // CIRQUEPINNACLE_ARCH_COMMON_H_
