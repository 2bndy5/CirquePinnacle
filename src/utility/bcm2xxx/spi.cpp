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
    #include <pthread.h>
    #include <unistd.h>
    #include <stdexcept>
    #include "spi.h"

    #ifdef __cplusplus
extern "C" {
    #endif

namespace cirque_pinnacle_arduino_wrappers {

static pthread_mutex_t spiMutex0 = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t spiMutex1 = PTHREAD_MUTEX_INITIALIZER;

SPIClass::SPIClass() : busID(PINNACLE_DEFAULT_SPI_BUS)
{
}

void SPIClass::begin(int busNumber)
{
    if (spiInitialized && busNumber != busID)
        end();
    if (busID >= 10)
        bcm2835_aux_spi_begin();
    else
        bcm2835_spi_begin();
    spiInitialized = true;
    busID = busNumber;
}

void SPIClass::end()
{
    if (busID >= 10)
        bcm2835_aux_spi_end();
    else
        bcm2835_spi_end();
    spiInitialized = false;
}

void SPIClass::beginTransaction(SPISettings settings)
{
    if (geteuid() != 0) {
        throw SPIException("Process should run as root");
    }
    if (busID >= 10)
        pthread_mutex_lock(&spiMutex1);
    else
        pthread_mutex_lock(&spiMutex0);
    bcm2835_spi_setBitOrder(settings.bitOrder);
    bcm2835_spi_setDataMode(settings.mode);
    bcm2835_spi_set_speed_hz(settings.clock);
    if (busID >= 10) {
        // bcm2835 lib does not provide support for chip select on the auxilary SPI bus.
        // brute force via GPIO API instead (not nearly as fast)
        digitalWrite(18 - (busID & 3), LOW);
    }
    else
        bcm2835_spi_chipSelect((uint8_t)(busID & 3));
}

void SPIClass::endTransaction()
{
    if (busID >= 10) {
        digitalWrite(18 - (busID & 3), HIGH);
        pthread_mutex_unlock(&spiMutex1);
    }
    else
        pthread_mutex_unlock(&spiMutex0);
}

void SPIClass::transfer(void* tx_buf, void* rx_buf, uint32_t len)
{
    if (busID >= 10)
        bcm2835_aux_spi_transfernb((char*)tx_buf, (char*)rx_buf, len);
    else
        bcm2835_spi_transfernb((char*)tx_buf, (char*)rx_buf, len);
}

void SPIClass::transfer(void* buf, uint32_t len)
{
    bcm2835_spi_transfern((char*)buf, len);
}

uint8_t SPIClass::transfer(uint8_t tx)
{
    if (busID >= 10)
        return bcm2835_aux_spi_transfer(tx);
    else
        return bcm2835_spi_transfer(tx);
}

SPIClass::~SPIClass()
{
    end();
}

SPIClass SPI;

} // namespace cirque_pinnacle_arduino_wrappers

    #ifdef __cplusplus
}
    #endif

#endif // !defined(ARDUINO)
