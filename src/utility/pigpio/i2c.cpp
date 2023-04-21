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
    #include <pigpio.h>
    #include "i2c.h"

    #ifdef __cplusplus
extern "C" {
    #endif

namespace cirque_pinnacle_arduino_wrappers {

    #ifndef PINNACLE_I2C_BUFFER_LENGTH
        #define PINNACLE_I2C_BUFFER_LENGTH 32
    #endif

    TwoWire::TwoWire()
        : xBuffIndex(0), xBuffLen(0), devHandle(-1), slaveAddress(0), i2cInitialized(false), busID(0)
    {
        xBuff = (uint8_t*)malloc(PINNACLE_I2C_BUFFER_LENGTH);
    }

    void TwoWire::begin(uint8_t bus_number, uint8_t address)
    {
        gpioInitialise();
        if (i2cInitialized || slaveAddress != address || busID != bus_number)
            end();
        devHandle = i2cOpen((unsigned int)bus_number, address, 0);
        if (devHandle >= 0) {
            i2cInitialized = true;
            slaveAddress = address;
            busID = bus_number;
        }
        else {
            throw I2CException("Failed to open I2C bus.");
        }
    }

    void TwoWire::end()
    {
        if (i2cInitialized) {
            i2cInitialized = false;

            i2cClose(devHandle);
            // PI_BAD_HANDLE can mean expired from prior destruction of pigpio interface
        }
    }

    void TwoWire::beginTransmission(uint8_t address)
    {
        if (!i2cInitialized || address != slaveAddress) {
            begin(busID, address);
        }
        xBuffLen = 0;
        xBuffIndex = 0;
    }

    uint8_t TwoWire::endTransmission(uint8_t sendStop)
    {
        (void)sendStop; // param not used in this wrapped implementation

        int reason = i2cWriteDevice(devHandle, (char*)xBuff, xBuffLen);
        if (reason != 0)
        {
            throw I2CException("I2C write operation failed.");
            return 0;
        }
        uint8_t retVal = xBuffLen;
        xBuffLen = 0;
        xBuffIndex = 0;
        // don't close the bus here as write commands should precede any read operation.
        return retVal;
    }

    uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity, uint8_t sendStop)
    {
        (void)sendStop; // param not used in this wrapped implementation

        if (quantity > PINNACLE_I2C_BUFFER_LENGTH)
            quantity = PINNACLE_I2C_BUFFER_LENGTH;

        // NOTE: Opening the I2C bus here may confuse the slave device because
        // unexpected data is used to initialize the bus (pigpio probes slave address).
        if (!i2cInitialized || address != slaveAddress) {
            begin(busID, address);
        }

        int response = i2cReadDevice(devHandle, (char*)xBuff, quantity);
        if (response <= 0) {
            throw I2CException("I2C read operation failed.");
            return 0;
        }
        xBuffLen = response;
        xBuffIndex = 0;
        // Typically, we would close the I2C bus here as the intention is to use the same
        // object for the entire bus (agnostic of slave devices).
        // The pigpio library treats I2C handles as objects specific to slave device address.
        // Instead of closing the bus here, we'll rely on `beginTransmission()` for speed.
        return response;
    }

    int TwoWire::available(void)
    {
        return xBuffLen - xBuffIndex;
    }

    int TwoWire::read(void)
    {
        return (int)xBuff[xBuffIndex++];
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

    TwoWire::~TwoWire()
    {
        free(xBuff);
        // end();
    }

    TwoWire Wire;

} // namespace cirque_pinnacle_arduino_wrappers

    #ifdef __cplusplus
}
    #endif

#endif // !defined(ARDUINO)
