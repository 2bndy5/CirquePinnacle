#ifndef ARDUINO

    #include <stdlib.h>
    #include "i2c.h"

namespace cirque_pinnacle_arduino_wrappers {

    #define PINNACLE_I2C_BUFFER_LENGTH 32

TwoWire::TwoWire()
    : slaveAddress(0), xBuffIndex(0), xBuffLen(0)
{
    xBuff = (uint8_t*)malloc(PINNACLE_I2C_BUFFER_LENGTH);
}

void TwoWire::begin(i2c_inst_t* hw_i2c, uint sda, uint scl)
{
    hw_id = hw_i2c;
    i2c_init(hw_id, 100000);
    gpio_set_function(sda, GPIO_FUNC_I2C);
    gpio_set_function(scl, GPIO_FUNC_I2C);
    gpio_pull_up(sda);
    gpio_pull_up(scl);
}

void TwoWire::end()
{
    i2c_deinit(hw_id);
}

void TwoWire::beginTransmission(uint8_t address)
{
    slaveAddress = address;
    xBuffLen = 0;
    xBuffIndex = 0;
}

uint8_t TwoWire::endTransmission(uint8_t sendStop)
{
    int result = i2c_write_timeout_us(hw_id, slaveAddress, xBuff, xBuffLen, !sendStop, 1000);
    if (result == PICO_ERROR_GENERIC) {
        // no I2C slave device present at specified address.
        return 0;
    }
    else if (result == PICO_ERROR_TIMEOUT) {
        // I2C read() operation timed out.
        return 0;
    }
    return (uint8_t)result;
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
    if (quantity > PINNACLE_I2C_BUFFER_LENGTH)
        quantity = PINNACLE_I2C_BUFFER_LENGTH;

    int result = i2c_read_timeout_us(hw_id, address, xBuff, quantity, !sendStop, 1000);
    xBuffIndex = 0;
    xBuffLen = 0;
    if (result == PICO_ERROR_GENERIC) {
        // "no I2C slave device present at specified address.
        return 0;
    }
    else if (result == PICO_ERROR_TIMEOUT) {
        // "I2C read() operation timed out.
        return 0;
    }
    xBuffLen = (uint8_t)result;
    return (uint8_t)result;
}

int TwoWire::available()
{
    if (xBuffIndex < xBuffLen) {
        return (xBuffLen - xBuffIndex);
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

    i2c_deinit(hw_id);
}

TwoWire Wire = TwoWire();

} // namespace cirque_pinnacle_arduino_wrappers

#endif // !defined(ARDUINO)
