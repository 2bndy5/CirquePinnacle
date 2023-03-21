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
#ifndef ARDUINO

    #include "spi.h"

namespace cirque_pinnacle_arduino_wrappers {

    #define PINNACLE_SPI_BYTE_SIZE 8

SPIClass::SPIClass()
{
}

void SPIClass::begin(spi_inst_t* hw_id, uint8_t _sck, uint8_t _tx, uint8_t _rx)
{
    _hw_id = hw_id;
    gpio_set_function(_sck, GPIO_FUNC_SPI);
    gpio_set_function(_tx, GPIO_FUNC_SPI);
    gpio_set_function(_rx, GPIO_FUNC_SPI);
    spi_init(_hw_id, PINNACLE_SPI_SPEED);
}

void SPIClass::end()
{
    spi_deinit(_hw_id);
}

void SPIClass::transfer(void* tx_buf, void* rx_buf, uint32_t len)
{
    spi_write_read_blocking(_hw_id, (const uint8_t*)tx_buf, (uint8_t*)rx_buf, len);
}

void SPIClass::transfer(void* buf, uint32_t len)
{
    spi_write_blocking(_hw_id, (const uint8_t*)buf, len);
}

uint8_t SPIClass::transfer(uint8_t tx)
{
    uint8_t recv = 0;
    spi_write_read_blocking(_hw_id, &tx, &recv, 1);
    return recv;
}

void SPIClass::beginTransaction(SPISettings spiSettings)
{
    spi_set_baudrate(_hw_id, spiSettings.clock);
    spi_set_format(_hw_id, PINNACLE_SPI_BYTE_SIZE, (spi_cpol_t)spiSettings.polarity, (spi_cpha_t)spiSettings.phase, spiSettings.bitOrder);
}

void SPIClass::endTransaction()
{
}

SPIClass::~SPIClass()
{
    end();
}

SPIClass SPI = SPIClass();

} // namespace cirque_pinnacle_arduino_wrappers

#endif // !defined(ARDUINO)
