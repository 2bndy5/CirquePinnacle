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

void SPIClass::transfernb(char* tx_buf, char* rx_buf, uint32_t len)
{
    spiXfer(spiHandle, tx_buf, rx_buf, len);
}

void SPIClass::transfern(char* buf, uint32_t len)
{
    transfernb(buf, buf, len);
}

uint8_t SPIClass::transfer(char tx)
{
    char rx_buf = 0;
    transfernb(&tx, &rx_buf, 1);
    return rx_buf;
}

SPIClass::~SPIClass()
{
}

SPIClass SPI = SPIClass();

} // namespace cirque_pinnacle_arduino_wrappers

#endif // !defined(ARDUINO)
