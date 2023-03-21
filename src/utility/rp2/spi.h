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
#ifndef CIRQUEPINNACLE_UTILITY_RP2_SPI_H_
#define CIRQUEPINNACLE_UTILITY_RP2_SPI_H_
#ifndef ARDUINO

    #include "pico/stdlib.h"
    #include "hardware/spi.h"

namespace cirque_pinnacle_arduino_wrappers {

    #define MSBFIRST SPI_MSB_FIRST
    #define LSBFIRST SPI_LSB_FIRST

    #define SPI_MODE0 0
    #define SPI_MODE1 1
    #define SPI_MODE2 2
    #define SPI_MODE3 3

    #ifndef PINNACLE_SPI_SPEED
        // Default is the maximum supported SPI speed (13 MHz)
        #define PINNACLE_SPI_SPEED 13000000
    #endif

    #ifdef PICO_DEFAULT_SPI_INSTANCE
        #define PINNACLE_DEFAULT_SPI_BUS PICO_DEFAULT_SPI_INSTANCE
    #else
        #define PINNACLE_DEFAULT_SPI_BUS spi0
    #endif

    // this SPI implements beginTransaction() & endTransaction() to
    // configure the SPI bus
    #define SPI_HAS_TRANSACTION 1

    #define PINNACLE_SS_CTRL(pin, value) digitalWrite(pin, value)

class SPISettings
{
public:
    SPISettings(uint32_t clock, spi_order_t bitOrder, uint8_t dataMode)
    {
        init(clock, bitOrder, dataMode);
    }

    SPISettings()
    {
        init(PINNACLE_SPI_SPEED, MSBFIRST, SPI_MODE1);
    }

    uint32_t clock;
    spi_order_t bitOrder;
    bool polarity;
    bool phase;

private:
    void init(uint32_t _clock, spi_order_t _bitOrder, uint8_t _dataMode)
    {
        clock = _clock;
        bitOrder = _bitOrder;
        polarity = (bool)((_dataMode >> 1) & 1);
        phase = (bool)(_dataMode & 1);
    }
};

class SPIClass
{

public:
    /** Instantiate an object for use with a single SPi bus. */
    SPIClass();

    /**
     * Initialize a certain SPI bus. Uses boards defaults when declared in the Pico SDK.
     * @param hw_id This is either `spi0` or `spi1` (provided by the Pico SDK)
     * @param _sck The pin to be used as the SPI bus' sck
     * @param _tx The pin to be used as the SPI bus' tx (MOSI)
     * @param _rx The pin to be used as the SPI bus' rx (MISO)
     *
     * @note This function assumes using the default SPI pins defined for your
     * board in "pico-sdk/src/boards/include/boards/*.h" files of the Pico SDK.
     * @see The [Pico SDK has a chart of applicable pins](https://datasheets.raspberrypi.org/pico/raspberry-pi-pico-c-sdk.pdf#%5B%7B%22num%22%3A106%2C%22gen%22%3A0%7D%2C%7B%22name%22%3A%22XYZ%22%7D%2C115%2C377.118%2Cnull%5D)
     * that can be used for hardware driven SPI transactions.
     */
    void begin(
        spi_inst_t* hw_id = PINNACLE_DEFAULT_SPI_BUS,
        uint8_t _sck = PICO_DEFAULT_SPI_SCK_PIN,
        uint8_t _tx = PICO_DEFAULT_SPI_TX_PIN,
        uint8_t _rx = PICO_DEFAULT_SPI_RX_PIN);

    /**
     * de-initialize the specified SPI bus
     */
    void end();

    /**
     * Transfer a buffer of bytes to/from a SPI slave device.
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
     * @note Any bytes received over MISO are discarded.
     */
    void transfer(void* buf, uint32_t len);

    /**
     * Transfer a single byte to/from a SPI slave device.
     * @param tx The byte to send over MOSI.
     * @return The byte received over MISO when sending the `tx` byte over MOSI.
     */
    uint8_t transfer(uint8_t tx);

    /**
     * Initialize and configure the SPI bus.
     * @param spiSettings A `SPISettings` object that encapsulates SPI bus configuration.
     */
    void beginTransaction(SPISettings spiSettings);

    /** Deinitialize the SPI bus. */
    void endTransaction();

    /** Clean-up any internal pointers/buffers/etc. */
    virtual ~SPIClass();

private:
    /** the ID of the hardware driven SPI bus */
    spi_inst_t* _hw_id;
};

// pre-instantiated SPI bus object (to use as a convenient default)
extern SPIClass SPI;

} // namespace cirque_pinnacle_arduino_wrappers

#endif // !defined(ARDUINO)
#endif // CIRQUEPINNACLE_UTILITY_RP2_SPI_H_
