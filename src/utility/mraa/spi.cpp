
#ifndef ARDUINO
    #include <cstdio>
    #include "spi.h"

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
    result = spi_inst->mode(settings.mode);
    if (result != mraa::Result::SUCCESS)
        throw SPIException("mraa::Spi::mode() failed.");
    result = spi_inst->bitPerWord((unsigned int)PINNACLE_SPI_BITS_PER_WORD);
    if (result != mraa::Result::SUCCESS)
        throw SPIException("mraa::Spi::bitPerWord() failed.");
    result = spi_inst->lsbmode(settings.bitOrder);
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

SPIClass SPI = SPIClass();

} // namespace cirque_pinnacle_arduino_wrappers

#endif // !defined(ARDUINO)
