#ifndef ARDUINO
    #include <pigpio.h>
    #include <cstring>
    #include <cstdio>
    #include "spi.h"

namespace cirque_pinnacle_arduino_wrappers {

SPIClass::SPIClass() : flags(0), busChannel(0), spiIsInitialized(false), spiSpeed(PINNACLE_SPI_SPEED)
{
}

void SPIClass::begin(uint8_t busNumber, uint32_t spi_speed)
{
    flags = (unsigned int)((busNumber / 10) << 8);
    spiSpeed = spi_speed;
    busChannel = (unsigned int)(busNumber & 2);
    init();
}

void SPIClass::init()
{
    if (spiIsInitialized) {
        end();
    }
    spiHandle = spiOpen(busChannel, spiSpeed, flags);
    if (spiHandle < 0) {
        switch (spiHandle) {
            case PI_BAD_SPI_CHANNEL:
                throw SPIException("pigpio failed to initialize the specified channel for the specified SPI bus");
                break;
            case PI_BAD_SPI_SPEED:
                throw SPIException("pigpio failed to use the specified SPI speed");
                break;
            case PI_BAD_FLAGS:
                throw SPIException("pigpio did not accept the specifed SPI flags");
                break;
            case PI_NO_AUX_SPI:
                throw SPIException("pigpio failed to initialize auxilary SPI bus");
                break;
            case PI_SPI_OPEN_FAILED:
                throw SPIException("pigpio failed to open SPI bus");
                break;
            default:
                throw SPIException("pigpio failed to initialize SPI handle");
                break;
        }
    }
    spiIsInitialized = true;
}

void SPIClass::end()
{
    if (!spiIsInitialized) {
        return;
    }
    int result = spiClose(spiHandle);
    if (result != 0) {
        throw SPIException("pigpio handle invalid");
    }
    spiIsInitialized = false;
}

void SPIClass::beginTransaction(SPISettings settings)
{
    unsigned int newFlags = flags & 0x80; // clear all flags but bus number
    newFlags |= settings.mode;
    newFlags |= settings.bitOrder;
    if (newFlags != flags || spiSpeed != settings.clock) {
        flags = newFlags;
        spiSpeed = settings.clock;
        init();
    }
}

void SPIClass::endTransaction()
{
}

void SPIClass::transfer(void* tx_buf, void* rx_buf, uint32_t len)
{
    int result = spiXfer(spiHandle, (char*)tx_buf, (char*)rx_buf, len);
    if (result == PI_BAD_HANDLE) {
        throw SPIException("pigpio handle not initialized");
    }
    else if (result == PI_BAD_SPI_COUNT) {
        throw SPIException("buffer length not bound by specified 'len' parameter");
    }
    else if (result == PI_SPI_XFER_FAILED) {
        throw SPIException("pigpio spiXfer() failed");
    }
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
    end();
}

SPIClass SPI = SPIClass();

} // namespace cirque_pinnacle_arduino_wrappers

#endif // !defined(ARDUINO)
