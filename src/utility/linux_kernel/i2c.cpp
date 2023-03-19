#ifndef ARDUINO

    #include <stdlib.h>
    #include <stdio.h>
    #include <fcntl.h>
    #include <unistd.h>
    #include <sys/ioctl.h>
    #include <linux/i2c.h>
    #include <linux/i2c-dev.h>
    #include "i2c.h"

namespace cirque_pinnacle_arduino_wrappers {

    #define PINNACLE_I2C_BUFFER_LENGTH 32

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

TwoWire Wire = TwoWire();

} // namespace cirque_pinnacle_arduino_wrappers

#endif // !defined(ARDUINO)
