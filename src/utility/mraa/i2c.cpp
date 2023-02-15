#ifndef ARDUINO
    #include <stdlib.h>
    #include <mraa/common.hpp>
    #include <mraa/i2c.hpp>
    #include "i2c.h"

namespace cirque_pinnacle_arduino_wrappers {

    #define PINNACLE_I2C_BUFFER_LENGTH 32

TwoWire::TwoWire()
    : xBuffIndex(0), xBuffLen(0)
{
    xBuff = (uint8_t*)malloc(PINNACLE_I2C_BUFFER_LENGTH);

    i2c_inst = nullptr;
}

void TwoWire::begin(uint8_t bus_number)
{
    i2c_inst = new mraa::I2c((int)bus_number);
}

void TwoWire::end()
{
    i2c_inst->close();
}

void TwoWire::beginTransmission(uint8_t address)
{
    i2c_inst->address(address);
    xBuffLen = 0;
    xBuffIndex = 0;
}

uint8_t TwoWire::endTransmission(uint8_t sendStop)
{
    int reason = i2c_inst->write((const uint8_t*)xBuff, xBuffLen);
    uint8_t retVal = xBuffLen;
    if (reason < 0)
    {
        throw I2CException("I2C write operation failed.");
        return 0;
    }
    xBuffLen = 0;
    xBuffIndex = 0;
    return retVal;
}

uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity, uint8_t sendStop)
{
    (void)address;  // param not used in this wrapped implementation
    (void)sendStop; // param not used in this wrapped implementation

    if (quantity > PINNACLE_I2C_BUFFER_LENGTH)
        quantity = PINNACLE_I2C_BUFFER_LENGTH;

    int response = i2c_inst->read(xBuff, quantity);
    if (response < 0)
    {
        throw I2CException("mraa::I2c.read() failed.");
        return 0;
    }
    xBuffLen = response;
    xBuffIndex = 0;
    return response;
}

int TwoWire::available(void)
{
    return xBuffLen - xBuffIndex;
}

int TwoWire::read(void)
{
    return (int)xBuff[++xBuffIndex];
}

size_t TwoWire::write(uint8_t data)
{
    if (xBuffIndex < PINNACLE_I2C_BUFFER_LENGTH) {
        xBuff[++xBuffIndex] = data;
        ++xBuffLen;
        return 1;
    }
    return 0;
}

TwoWire::~TwoWire()
{
    free(xBuff);
    i2c_inst->close();
    delete i2c_inst;
}

TwoWire Wire = TwoWire();

} // namespace cirque_pinnacle_arduino_wrappers

#endif // !defined(ARDUINO)
