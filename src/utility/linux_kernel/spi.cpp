#ifndef ARDUINO
    #include <fcntl.h>
    #include <linux/spi/spidev.h>
    #include <memory.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <sys/ioctl.h>
    #include <unistd.h>
    #include "spi.h"

namespace cirque_pinnacle_arduino_wrappers {

    #define PINNACLE_SPI_BITS_PER_WORD 8

void debug_printf(uint8_t* buf, uint32_t len)
{
    for (uint32_t i = 0; i < len; ++i) {
        printf(" 0x%02X", (unsigned int)(buf[i]));
    }
}

SPIClass::SPIClass()
    : fd(-1), _spi_speed(PINNACLE_SPI_SPEED)
{
}

void SPIClass::begin(int busNumber, uint32_t spi_speed)
{

    if (spiIsInitialized) {
        return;
    }

    /* set spidev accordingly to busNumber like:
     * busNumber = 23 -> /dev/spidev2.3
     */
    char device[] = "/dev/spidev0.0";
    device[11] += (busNumber / 10) % 10;
    device[13] += busNumber % 10;

    if (fd >= 0) // check whether spi is already open
    {
        close(fd);
        fd = -1;
    }

    fd = open(device, O_RDWR);
    if (fd < 0) {
        throw SPIException("can't open device");
    }
    spiIsInitialized = true;
    init(spi_speed);
}

void SPIClass::init(uint32_t speed)
{
    uint8_t bits = PINNACLE_SPI_BITS_PER_WORD;
    uint8_t mode = SPI_MODE_1;

    int ret;

    /*
     * spi mode
     */
    ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);
    if (ret == -1) {
        throw SPIException("can't set WR spi mode");
    }

    ret = ioctl(fd, SPI_IOC_RD_MODE, &mode);
    if (ret == -1) {
        throw SPIException("can't set RD spi mode");
    }

    /*
     * bits per word
     */
    ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
    if (ret == -1) {
        throw SPIException("can't set WR bits per word");
    }

    ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
    if (ret == -1) {
        throw SPIException("can't set RD bits per word");
    }

    /*
     * max speed hz
     */
    ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
    if (ret == -1) {
        throw SPIException("can't WR set max speed hz");
    }

    ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
    if (ret == -1) {
        throw SPIException("can't RD set max speed hz");
    }

    _spi_speed = speed;
}

uint8_t SPIClass::transfer(uint8_t tx)
{
    struct spi_ioc_transfer tr;
    memset(&tr, 0, sizeof(tr));
    tr.tx_buf = (unsigned long)&tx;
    uint8_t rx;
    tr.rx_buf = (unsigned long)&rx;
    tr.len = sizeof(tx);
    tr.speed_hz = _spi_speed;
    tr.delay_usecs = 0;
    tr.bits_per_word = PINNACLE_SPI_BITS_PER_WORD;
    tr.cs_change = 0;

    int ret;
    ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
    if (ret < 1) {
        throw SPIException("can't send spi message");
    }
    printf("SPI out:");
    debug_printf(&tx, 1);
    printf(" in:");
    debug_printf(&rx, 1);
    printf("\n");
    return rx;
}

void SPIClass::transfer(void* tx_buf, void* rx_buf, uint32_t len)
{
    struct spi_ioc_transfer tr;
    memset(&tr, 0, sizeof(tr));
    tr.tx_buf = (unsigned long)tx_buf;
    tr.rx_buf = (unsigned long)rx_buf;
    tr.len = len;
    tr.speed_hz = _spi_speed;
    tr.delay_usecs = 0;
    tr.bits_per_word = PINNACLE_SPI_BITS_PER_WORD;
    tr.cs_change = 0;

    int ret;
    ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
    if (ret < 1) {
        throw SPIException("can't send spi message");
    }
    printf("SPI out:");
    debug_printf((uint8_t*)tx_buf, len);
    printf(" in:");
    debug_printf((uint8_t*)rx_buf, len);
    printf("\n");
}

void SPIClass::transfer(void* buf, uint32_t len)
{
    transfer(buf, buf, len);
}

SPIClass::~SPIClass()
{
    if (fd >= 0) {
        close(fd);
    }
}

SPIClass SPI = SPIClass();

} // namespace cirque_pinnacle_arduino_wrappers

#endif // !defined(ARDUINO)
