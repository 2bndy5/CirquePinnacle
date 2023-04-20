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
    #include <cstdio>
    #include "spi.h"

    #ifdef __cplusplus
extern "C" {
    #endif

namespace cirque_pinnacle_arduino_wrappers {

    #define PINNACLE_SPI_BITS_PER_WORD 8

    SPIClass::SPIClass()
        : spi_inst(nullptr)
    {
    }

    void SPIClass::begin(int busNumber, SPISettings settings)
    {
        // mraa::Spi() uses the default chip select available for the specified bus number on the utilized platform
        // On RPi, mraa only supports the primary SPI bus 0 & seems to only use CE0 (GPIO8)
        spi_inst = new mraa::Spi(busNumber / 10); // reduce the busNumber to only the bus ID (excluding CE number)
        mraa::Result result;
        result = spi_inst->mode((mraa::Spi_Mode)settings.mode);
        if (result != mraa::Result::SUCCESS)
            throw SPIException("mraa::Spi::mode() failed.");
        result = spi_inst->bitPerWord((unsigned int)PINNACLE_SPI_BITS_PER_WORD);
        if (result != mraa::Result::SUCCESS)
            throw SPIException("mraa::Spi::bitPerWord() failed.");
        result = spi_inst->lsbmode((bool)settings.bitOrder);
        if (result != mraa::Result::SUCCESS)
            throw SPIException("mraa::Spi::lsbmode() failed.");
        result = spi_inst->frequency(settings.clock);
        if (result != mraa::Result::SUCCESS)
            throw SPIException("mraa::Spi::frequency() failed.");
    }

    void SPIClass::end()
    {
        if (spi_inst != nullptr) {
            spi_inst->close();
            spi_inst = nullptr;
        }
    }

    void SPIClass::transfer(void* tx_buf, void* rx_buf, uint32_t len)
    {
        mraa::Result result = spi_inst->transfer((uint8_t*)tx_buf, (uint8_t*)rx_buf, len);
        if (result != mraa::Result::SUCCESS)
            throw SPIException("mraa::Spi::transfer() failed");
    }

    void SPIClass::transfer(void* buf, uint32_t len)
    {
        uint8_t* rx_buf = (uint8_t*)malloc(len);
        memset(rx_buf, 0xFB, len);
        transfer(buf, rx_buf, len);
        memcpy(buf, rx_buf, len);
        free(rx_buf);
    }

    uint8_t SPIClass::transfer(uint8_t tx)
    {
        return spi_inst->writeByte(tx);
    }

    SPIClass::~SPIClass()
    {
        end();
        delete spi_inst;
    }

    SPIClass SPI;

} // namespace cirque_pinnacle_arduino_wrappers

    #ifdef __cplusplus
}
    #endif

#endif // !defined(ARDUINO)
