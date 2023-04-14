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
    #include <cstring>
    #include <cstdio>
    #include <pigpio.h>
    #include "spi.h"

    #ifdef __cplusplus
extern "C" {
    #endif

namespace cirque_pinnacle_arduino_wrappers {

    SPIClass::SPIClass() : spiHandle(-1), spiIsInitialized(false)
    {
    }

    void SPIClass::begin(uint8_t busNumber, SPISettings settings)
    {
        unsigned int flags = (unsigned int)((busNumber / 10) << 8);
        flags |= settings.bitOrder;
        flags |= settings.mode;
        unsigned int busChannel = (unsigned int)(busNumber & 2);
        if (spiIsInitialized) {
            end();
        }
        spiHandle = spiOpen(busChannel, settings.clock, flags);
        if (spiHandle == PI_BAD_SPI_CHANNEL)
            throw SPIException("pigpio failed to initialize the specified channel for the specified SPI bus");
        else if (spiHandle == PI_BAD_SPI_SPEED)
            throw SPIException("pigpio failed to use the specified SPI speed");
        else if (spiHandle == PI_BAD_FLAGS)
            throw SPIException("pigpio did not accept the specifed SPI flags");
        else if (spiHandle == PI_NO_AUX_SPI)
            throw SPIException("pigpio failed to initialize auxilary SPI bus");
        else if (spiHandle == PI_SPI_OPEN_FAILED)
            throw SPIException("pigpio failed to open SPI bus");
        spiIsInitialized = true;
    }

    void SPIClass::end()
    {
        if (!spiIsInitialized) {
            return;
        }
        int result = spiClose(spiHandle);
        spiIsInitialized = false;
        if (result != 0) {
            throw SPIException("pigpio handle invalid");
        }
    }

    void SPIClass::transfer(void* tx_buf, void* rx_buf, uint32_t len)
    {
        int result = spiXfer(spiHandle, (char*)tx_buf, (char*)rx_buf, len);
        if (result == PI_BAD_HANDLE)
            throw SPIException("pigpio handle not initialized");
        else if (result == PI_BAD_SPI_COUNT)
            throw SPIException("buffer length not bound by specified 'len' parameter");
        else if (result == PI_SPI_XFER_FAILED)
            throw SPIException("pigpio spiXfer() failed");
    }

    void SPIClass::transfer(void* buf, uint32_t len)
    {
        uint8_t* rx_buf = (uint8_t*)malloc(len);
        memset(rx_buf, 0, len);
        transfer(buf, rx_buf, len);
        memcpy(buf, rx_buf, len);
        free(rx_buf);
    }

    uint8_t SPIClass::transfer(uint8_t tx)
    {
        uint8_t rx_buf = 0;
        transfer((void*)(&tx), (void*)(&rx_buf), 1);
        return rx_buf;
    }

    SPIClass::~SPIClass()
    {
    }

    SPIClass SPI;

} // namespace cirque_pinnacle_arduino_wrappers

    #ifdef __cplusplus
}
    #endif

#endif // !defined(ARDUINO)
