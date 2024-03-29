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
    #include <stdint.h>    // uintXX_ts
    #include <unistd.h>    // close()
    #include <fcntl.h>     // open()
    #include <sys/ioctl.h> // ioctl()
    #include <errno.h>     // errno,
    #include <string.h>    // memset() strerror()
    #include <linux/spi/spidev.h>
    #include "spi.h"

    #ifdef __cplusplus
extern "C" {
    #endif

namespace cirque_pinnacle_arduino_wrappers {

    #define PINNACLE_SPI_BITS_PER_WORD 8

    SPIClass::SPIClass()
        : fd(-1), _spi_speed(PINNACLE_SPI_SPEED)
    {
    }

    void SPIClass::begin(int busNumber, SPISettings settings)
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
            std::string msg = "[SPIClass::begin] Could not open device ";
            msg += device;
            msg += "; ";
            msg += strerror(errno);
            throw SPIException(msg);
        }
        spiIsInitialized = true;

        int ret;

        // spi mode
        ret = ioctl(fd, SPI_IOC_WR_MODE, &settings.mode);
        if (ret == -1) {
            std::string msg = "[SPIClass::begin] Could not set write mode; ";
            msg += strerror(errno);
            throw SPIException(msg);
        }

        ret = ioctl(fd, SPI_IOC_RD_MODE, &settings.mode);
        if (ret == -1) {
            std::string msg = "[SPIClass::begin] Could not set read mode; ";
            msg += strerror(errno);
            throw SPIException(msg);
        }

        // bits per word
        uint8_t bits = PINNACLE_SPI_BITS_PER_WORD;
        ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
        if (ret == -1) {
            std::string msg = "[SPIClass::begin] Could not set write bits per word; ";
            msg += strerror(errno);
            throw SPIException(msg);
        }

        ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
        if (ret == -1) {
            std::string msg = "[SPIClass::begin] Could not set read bits per word; ";
            msg += strerror(errno);
            throw SPIException(msg);
        }

        // max speed Hz
        ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &settings.clock);
        if (ret == -1) {
            std::string msg = "[SPIClass::begin] Could not set write max speed Hz; ";
            msg += strerror(errno);
            throw SPIException(msg);
        }

        ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &settings.clock);
        if (ret == -1) {
            std::string msg = "[SPIClass::begin] Could not set read max speed Hz; ";
            msg += strerror(errno);
            throw SPIException(msg);
        }

        _spi_speed = settings.clock;
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
            std::string msg = "[SPIClass::transfer(tx)] Could not transfer buffer; ";
            msg += strerror(errno);
            throw SPIException(msg);
        }
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
            std::string msg = "[SPIClass::transfer(tx, rx, len)] Could not transfer buffer; ";
            msg += strerror(errno);
            throw SPIException(msg);
        }
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

    SPIClass SPI;

} // namespace cirque_pinnacle_arduino_wrappers

    #ifdef __cplusplus
}
    #endif

#endif // !defined(ARDUINO)
