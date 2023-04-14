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
    #include "i2c.h"

    #ifdef __cplusplus
extern "C" {
    #endif

namespace cirque_pinnacle_arduino_wrappers {

    #ifndef PINNACLE_I2C_BUFFER_LENGTH
        #define PINNACLE_I2C_BUFFER_LENGTH 32
    #endif

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
        int result = i2c_write_blocking(hw_id, slaveAddress, xBuff, xBuffLen, !sendStop);
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

        int result = i2c_read_blocking(hw_id, address, xBuff, quantity, !sendStop);
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

    #ifdef __cplusplus
}
    #endif

#endif // !defined(ARDUINO)
