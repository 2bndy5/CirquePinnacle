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
}

void SPIClass::transfernb(const uint8_t* tx_buf, uint8_t* rx_buf, uint32_t len)
{
    spi_write_read_blocking(_hw_id, tx_buf, rx_buf, len);
}

void SPIClass::transfern(const uint8_t* buf, uint32_t len)
{
    spi_write_blocking(_hw_id, buf, len);
}

uint8_t SPIClass::transfer(uint8_t tx)
{
    uint8_t recv = 0;
    spi_write_read_blocking(_hw_id, &tx, &recv, 1);
    return recv;
}

void SPIClass::beginTransaction(SPISettings spiSettings)
{
    spi_init(_hw_id, spiSettings.clock);
    spi_set_format(_hw_id, PINNACLE_SPI_BYTE_SIZE, (spi_cpol_t)spiSettings.polarity, (spi_cpha_t)spiSettings.phase, spiSettings.bitOrder);
}

void SPIClass::endTransaction()
{
    spi_deinit(_hw_id);
}

SPIClass::~SPIClass()
{
}

SPIClass SPI = SPIClass();

} // namespace cirque_pinnacle_arduino_wrappers

#endif // !defined(ARDUINO)
