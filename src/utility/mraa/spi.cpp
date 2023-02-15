
#ifndef ARDUINO
    #include "spi.h"
    #include "mraa.h"

namespace cirque_pinnacle_arduino_wrappers {

SPIClass::SPIClass()
    : spi_inst(nullptr)
{
}

void SPIClass::begin(int busNumber)
{
    // init mraa spi bus, it will handle chip select internally. For CS pin wiring user must check SPI details in hardware manual
    spi_inst = new mraa::Spi(busNumber);
}

void SPIClass::end()
{
    if (spi_inst != nullptr) {
        spi_inst->close();
        spi_inst = nullptr;
    }
}

void SPIClass::beginTransaction(SPISettings spiSettings)
{
    spi_inst->mode(spiSettings.mode);
    spi_inst->bitPerWord(8);
    spi_inst->frequency(spiSettings.clock);
    spi_inst->mode(spiSettings.mode);
}

void SPIClass::endTransaction()
{
}

void SPIClass::transfernb(char* tx_buf, char* rx_buf, uint32_t len)
{
    spi_inst->transfer((uint8_t*)tx_buf, (uint8_t*)rx_buf, len);
}

void SPIClass::transfern(char* buf, uint32_t len)
{
    transfernb(buf, buf, len);
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
