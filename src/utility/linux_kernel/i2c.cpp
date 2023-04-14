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

    #include <stdlib.h>
    #include <stdio.h>
    #include <fcntl.h>
    #include <unistd.h>
    #include <sys/ioctl.h>
    #include <linux/i2c.h>
    #include <linux/i2c-dev.h>
    #include "i2c.h"

    #ifdef __cplusplus
extern "C" {
    #endif

namespace cirque_pinnacle_arduino_wrappers {

    #ifndef PINNACLE_I2C_BUFFER_LENGTH
        #define PINNACLE_I2C_BUFFER_LENGTH 32
    #endif

    TwoWire::TwoWire()
        : xBuffIndex(0), xBuffLen(0), bus_fd(-1)
    {
        xBuff = (uint8_t*)malloc(PINNACLE_I2C_BUFFER_LENGTH);
    }

    void TwoWire::begin(uint8_t busNumber)
    {
        int file;
        char filename[13];

        sprintf(filename, "/dev/i2c-%d", busNumber);
        file = open(filename, O_RDWR);
        if (file < 0) {
            throw I2CException("Can't open I2C bus. Check access rights.");
        }
        bus_fd = file;
    }

    void TwoWire::end()
    {
        close(bus_fd);
        bus_fd = -1;
    }

    void TwoWire::beginTransmission(uint8_t address)
    {
        if (ioctl(bus_fd, I2C_SLAVE, address) < 0) {
            throw I2CException("Could not select I2C slave address.");
        }
        xBuffIndex = 0;
        xBuffLen = 0;
    }

    uint8_t TwoWire::endTransmission(uint8_t sendStop)
    {
        (void)sendStop; // param not used in this implementation

        if (::write(bus_fd, xBuff, xBuffLen) != xBuffLen) {
            throw I2CException("Could not write data to I2C bus.");
        }
        return xBuffLen;
    }

    size_t TwoWire::write(uint8_t data)
    {
        if (xBuffIndex < PINNACLE_I2C_BUFFER_LENGTH) {
            xBuff[xBuffIndex++] = data;
            xBuffLen++;
            return 1;
        }
        return 0;
    }

    uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity, uint8_t sendStop)
    {
        (void)sendStop; // param not used in this implementation

        if (quantity > PINNACLE_I2C_BUFFER_LENGTH) {
            quantity = PINNACLE_I2C_BUFFER_LENGTH;
        }

        xBuffIndex = 0;
        int retVal = ::read(bus_fd, xBuff, quantity);
        if (retVal < 0) {
            throw I2CException("Could not read data from I2C bus.");
        }
        xBuffLen = quantity;
        return xBuffLen;
    }

    int TwoWire::available()
    {
        if (xBuffIndex < xBuffLen) {
            return xBuffLen - xBuffIndex;
        }
        return 0;
    }

    int TwoWire::read()
    {
        if (xBuffIndex < xBuffLen) {
            return xBuff[xBuffIndex++];
        }
        return -1;
    }

    TwoWire::~TwoWire()
    {
        free(xBuff);
        end();
    }

    TwoWire Wire;

} // namespace cirque_pinnacle_arduino_wrappers

    #ifdef __cplusplus
}
    #endif

#endif // !defined(ARDUINO)
