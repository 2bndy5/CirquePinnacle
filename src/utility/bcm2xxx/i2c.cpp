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
    #include "bcm2835.h"

    #ifdef __cplusplus
extern "C" {
    #endif

namespace cirque_pinnacle_arduino_wrappers {

    #ifndef PINNACLE_I2C_BUFFER_LENGTH
        #define PINNACLE_I2C_BUFFER_LENGTH 32
    #endif

    TwoWire::TwoWire()
        : xBuffIndex(0), xBuffLen(0)
    {
        xBuff = (uint8_t*)malloc(PINNACLE_I2C_BUFFER_LENGTH);
    }

    void TwoWire::begin()
    {
        if (!bcm2835_i2c_begin()) {
            throw I2CException("Could not initialize the I2C pins (requires root privilege).");
        }
    }

    void TwoWire::end()
    {
        bcm2835_i2c_end();
    }

    void TwoWire::beginTransmission(uint8_t address)
    {
        bcm2835_i2c_setSlaveAddress(address);
        xBuffLen = 0;
        xBuffIndex = 0;
    }

    uint8_t TwoWire::endTransmission(uint8_t sendStop)
    {
        (void)sendStop; // param not used in this implementation

        uint8_t reason = bcm2835_i2c_write((const char*)xBuff, xBuffLen);
        uint8_t retVal = xBuffLen;
        switch (reason)
        {
            case BCM2835_I2C_REASON_OK:
                xBuffLen = 0;
                xBuffIndex = 0;
                return retVal;
            case BCM2835_I2C_REASON_ERROR_NACK:
                throw I2CException("Received an unexpected NACK.");
                return 0;
            case BCM2835_I2C_REASON_ERROR_CLKT:
                throw I2CException("Received a clock stretch timeout.");
                return 0;
            case BCM2835_I2C_REASON_ERROR_DATA:
                throw I2CException("Not all data was sent or received.");
                return 0;
            default:
                // We shouldn't reach here.
                // If we do, then something went wrong and bcm2835 lib doesn't recognize the error.
                return 0;
        }
    }

    uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity, uint8_t sendStop)
    {
        (void)address;  // param not used in this wrapped implementation
        (void)sendStop; // param not used in this wrapped implementation

        if (quantity > PINNACLE_I2C_BUFFER_LENGTH)
            quantity = PINNACLE_I2C_BUFFER_LENGTH;

        uint8_t reason = bcm2835_i2c_read((char*)xBuff, quantity);
        switch (reason)
        {
            case BCM2835_I2C_REASON_OK:
                xBuffLen = quantity;
                xBuffIndex = 0;
                return quantity;
            case BCM2835_I2C_REASON_ERROR_NACK:
                throw I2CException("Received an unexpected NACK.");
                return 0;
            case BCM2835_I2C_REASON_ERROR_CLKT:
                throw I2CException("Received a clock stretch timeout.");
                return 0;
            case BCM2835_I2C_REASON_ERROR_DATA:
                throw I2CException("Not all data was sent or received.");
                return 0;
            default:
                // We shouldn't reach here.
                // If we do, then something went wrong and bcm2835 lib doesn't recognize the error.
                return 0;
        }
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
        end();
    }

    TwoWire Wire;

} // namespace cirque_pinnacle_arduino_wrappers

    #ifdef __cplusplus
}
    #endif

#endif // !defined(ARDUINO)
