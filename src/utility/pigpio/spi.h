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
#ifndef CIRQUEPINNACLE_UTILITY_PIGPIO_SPI_H_
#define CIRQUEPINNACLE_UTILITY_PIGPIO_SPI_H_
#ifndef ARDUINO

    #include <cstdint>
    #include <stdexcept>
    #include "gpio.h"

    #ifdef __cplusplus
extern "C" {
    #endif

namespace cirque_pinnacle_arduino_wrappers {

    #ifndef PINNACLE_SPI_SPEED
        // Default is the recommended 6 MHz (maximum supported SPI speed is 13 MHz)
        #define PINNACLE_SPI_SPEED 6000000
    #endif

    #ifndef PINNACLE_DEFAULT_SPI_BUS
        // Default is RPi's CE0 pin on the primary bus 0: /dev/spidev0.0
        #define PINNACLE_DEFAULT_SPI_BUS 0
    #endif

    #define PINNACLE_SS_CTRL(pin, value)
    #define PINNACLE_USE_NATIVE_CS

    #define PINNACLE_SPI_BUFFER_OPS 1

    enum BitOrder : int
    {
        MSBFIRST = 0,
        LSBFIRST = (1 << 14)
    };

    enum DataMode : uint8_t
    {
        SPI_MODE0 = 0,
        SPI_MODE1 = 1,
        SPI_MODE2 = 2,
        SPI_MODE3 = 3
    };

    /** Specific exception for SPI errors */
    class SPIException : public std::runtime_error
    {
    public:
        explicit SPIException(const std::string& msg)
            : std::runtime_error(msg)
        {
        }
    };

    class SPISettings
    {
    public:
        SPISettings(uint32_t clock, uint8_t bitOrder, uint8_t dataMode)
        {
            init(clock, bitOrder, dataMode);
        }

        SPISettings()
        {
            init(PINNACLE_SPI_SPEED, MSBFIRST, SPI_MODE1);
        }

        uint32_t clock;
        unsigned int bitOrder;
        unsigned int mode;

    private:
        void init(uint32_t _clock, unsigned int _bitOrder, unsigned int _dataMode)
        {
            clock = _clock;
            bitOrder = _bitOrder;
            mode = _dataMode;
        }
    };

    class SPIClass
    {

    public:
        /** Instantiate an object for use with a single SPi bus. */
        SPIClass();

        /**
         * Initialize a certain SPI bus. Uses boards defaults when available.
         * @param busNumber This is the SPI bus number and corresponding channel (CEx pin on RPi).
         * | bus ID | CE number | param value | spidev device  |
         * |:------:|:---------:|:-----------:|:--------------:|
         * |    0   |     0     |      0      | /dev/spidev0.0 |
         * |    0   |     1     |      1      | /dev/spidev0.1 |
         * |    1   |     0     |     10      | /dev/spidev1.0 |
         * |    1   |     1     |     11      | /dev/spidev1.1 |
         * |    1   |     2     |     12      | /dev/spidev1.2 |
         * @param settings An object used to specify the baudrate (aka frequency), Endianess, and clock
         *     polarity/phase to be used on the specified SPI bus.
         */
        void begin(uint8_t busNumber = PINNACLE_DEFAULT_SPI_BUS, SPISettings settings = SPISettings());

        /** De-initialize the SPI bus specified to `SPIClass::begin()` */
        void end();

        /**
         * Transfer buffers of bytes to/from a SPI slave device.
         * @param tx_buf The pointer to a buffer of bytes to send over MOSI.
         * @param rx_buf The pointer to a buffer of bytes that get received over MISO.
         * @param len The length of each buffer of bytes; each buffer should have equal length.
         */
        void transfer(void* tx_buf, void* rx_buf, uint32_t len);

        /**
         * Transfer a buffer of bytes to a SPI slave device.
         * @param buf The pointer to a buffer of bytes to send over MOSI.
         * @param len The length of the buffer of bytes.
         *
         * @note The bytes received over MISO will replace the
         * buffer contents as the bytes are sent over MOSI.
         */
        void transfer(void* buf, uint32_t len);

        /**
         * Transfer a buffer of bytes to a SPI slave device.
         * @param tx The pointer to a buffer of bytes to send over MOSI.
         * @return The byte received over MISO when sending the `tx` byte over MOSI.
         */
        uint8_t transfer(uint8_t tx);

        /** Clean-up any internal pointers/buffers/etc. */
        ~SPIClass();

    private:
        int spiHandle;
        bool spiIsInitialized = false;
    };

    // pre-instantiated SPI bus object (to use as a convenient default)
    extern SPIClass SPI;

} // namespace cirque_pinnacle_arduino_wrappers

    #ifdef __cplusplus
}
    #endif

#endif // !defined(ARDUINO)
#endif // CIRQUEPINNACLE_UTILITY_PIGPIO_SPI_H_
