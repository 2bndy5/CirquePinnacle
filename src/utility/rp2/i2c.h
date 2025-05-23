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
#ifndef CIRQUEPINNACLE_UTILITY_RP2_I2C_H_
#define CIRQUEPINNACLE_UTILITY_RP2_I2C_H_
#ifndef ARDUINO

    #include <cstdint>
    #include "pico/stdlib.h"
    #include "hardware/i2c.h"

    #ifdef __cplusplus
extern "C" {
    #endif

namespace cirque_pinnacle_arduino_wrappers {

    #ifdef PICO_DEFAULT_I2C_INSTANCE
        #define PINNACLE_DEFAULT_I2C_BUS PICO_DEFAULT_I2C_INSTANCE()
    #else
        #define PINNACLE_DEFAULT_I2C_BUS i2c0
    #endif
    class TwoWire
    {

    public:
        /** Instantiate the object for use with the I2C bus. */
        TwoWire();

        /** Initialize the I2C bus' pins. */
        void begin(i2c_inst_t* hw_i2c = PINNACLE_DEFAULT_I2C_BUS, uint sda = PICO_DEFAULT_I2C_SDA_PIN, uint scl = PICO_DEFAULT_I2C_SCL_PIN);

        /** De-initialize the I2C bus' pins. */
        void end();

        /**
         * Start a transaction over the bus.
         * @note This will reset the internal buffer used for write().
         * @param address The slave device's I2C address.
         */
        void beginTransmission(uint8_t address);

        /**
         * Terminate transactions on the bus.
         *
         * This function is used to send the bytes in the internal buffer (set with `write()`) over
         * the bus with an optional stop condition.
         *
         * @note This will also reset the internal buffer.
         * @param sendStop If set to a non-zero value, then a stop condition is sent over the bus.
         * @return The number of bytes sent over the bus.
         */
        uint8_t endTransmission(uint8_t sendStop = false);

        /**
         * Write a single byte to the bus.
         * @note This must be called after `beginTransmission()`. After all bytes are ready to be sent,
         * use `endTransmission()` to send the internal buffer over the bus.
         * @returns The number of bytes written to the internal buffer.
         */
        size_t write(uint8_t data);

        /**
         * Request a number of bytes to read from the specified I2C device's address.
         * @param address The slave device's I2C address.
         * To specify the register offset, use beginTransmission() and write() the register offset.
         * @param quantity The number of bytes to request.
         * @param sendStop If set to a non-zero value, then a stop condition is sent over the bus
         * after the transaction is completed.
         * @return The number of bytes read into the internal buffer.
         */
        uint8_t requestFrom(uint8_t address, uint8_t quantity, uint8_t sendStop);

        /**
         * Check for remaining bytes in the internal buffer.
         * @note This must be called after `requestFrom()` in order to capture bytes from the bus into
         * the internal buffer.
         * @return the number of bytes left in the buffer. A negative number (or zero) means
         * all data from buffer has been `read()`.
         */
        int available();

        /**
         * Read a byte from the internal buffer.
         * @returns The next single byte from the internal buffer.
         * Use `available()` to prevent out of bounds and/or segmentation fault errors.
         */
        int read();

        /** free up the allocated memory for internal buffer */
        virtual ~TwoWire();

    private:
        i2c_inst_t* hw_id;
        uint8_t slaveAddress;
        uint8_t* xBuff;
        uint8_t xBuffIndex;
        uint8_t xBuffLen;
    };

    // pre-instantiated I2C bus object (to use as a convenient default)
    extern TwoWire Wire;

} // namespace cirque_pinnacle_arduino_wrappers

    #ifdef __cplusplus
}
    #endif

#endif // !defined(ARDUINO)
#endif // CIRQUEPINNACLE_UTILITY_RP2_I2C_H_
