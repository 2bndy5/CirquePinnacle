#ifndef ARDUINO
    #include <pigpio.h>
    #include "spi.h"

namespace cirque_pinnacle_arduino_wrappers {

SPIClass::SPIClass()
{
}

void SPIClass::begin(uint8_t busNumber, uint32_t spi_speed)
{
    if (this->spiIsInitialized) {
        return;
    }
    spiIsInitialized = true;
    gpioInitialise();
    spiHandle = spiOpen((unsigned int)(busNumber & 2), spi_speed, (unsigned int)((busNumber / 10) << 7));
}

void SPIClass::init(uint32_t speed)
{
}

void SPIClass::transfer(void* tx_buf, void* rx_buf, uint32_t len)
{
    spiXfer(spiHandle, (char*)tx_buf, (char*)rx_buf, len);
}

void SPIClass::transfer(void* buf, uint32_t len)
{
    transfer(buf, buf, len);
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

SPIClass SPI = SPIClass();

} // namespace cirque_pinnacle_arduino_wrappers

#endif // !defined(ARDUINO)
