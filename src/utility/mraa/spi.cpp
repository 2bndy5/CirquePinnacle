
#ifndef ARDUINO
    #include <cstdio>
    #include "spi.h"
    #include "mraa.h"

namespace cirque_pinnacle_arduino_wrappers {

    #define PINNACLE_SPI_BITS_PER_WORD 8

void debug_printf(uint8_t* buf, uint32_t len)
{
    for (uint32_t i = 0; i < len; ++i)
        printf(" %02X", (unsigned int)(buf[i]));
}

SPIClass::SPIClass()
    : spi_inst(nullptr)
{
}

void SPIClass::begin(int busNumber)
{
    // init mraa spi bus, it will handle chip select internally.
    int bus = busNumber / 10;
    int cs = busNumber & 3;
    spi_inst = new mraa::Spi(bus, cs);
}

void SPIClass::end()
{
    if (spi_inst != nullptr) {
        spi_inst->close();
        spi_inst = nullptr;
    }
}

void SPIClass::beginTransaction(SPISettings settings)
{
    spi_inst->mode(settings.mode);
    spi_inst->bitPerWord(PINNACLE_SPI_BITS_PER_WORD);
    spi_inst->lsbmode(settings.bitOrder);
    spi_inst->frequency(settings.clock);
}

void SPIClass::endTransaction()
{
}

void SPIClass::transfer(void* tx_buf, void* rx_buf, uint32_t len)
{
    mraa::Result result = spi_inst->transfer((uint8_t*)tx_buf, (uint8_t*)rx_buf, len);
    printf("SPI out:");
    debug_printf((uint8_t*)tx_buf, len);
    printf(" in:");
    debug_printf((uint8_t*)rx_buf, len);
    printf("\n");
    if (result != mraa::Result::SUCCESS)
        throw SPIException("mraa::Spi::transfer() failed");
}

void SPIClass::transfer(void* buf, uint32_t len)
{
    transfer(buf, buf, len);
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
