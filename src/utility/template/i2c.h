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
#ifndef CIRQUEPINNACLE_UTILITY_TEMPLATE_I2C_H_
#define CIRQUEPINNACLE_UTILITY_TEMPLATE_I2C_H_
#ifndef ARDUINO

    #include <cstdint>
    #include <stdexcept>

namespace cirque_pinnacle_arduino_wrappers {

    #ifndef PINNACLE_DEFAULT_I2C_BUS
        /**
         * Convenient alias to control which I2C bus (on Linux) is used by default.
         * Use Cmake option: `-D PINNACLE_DEFAULT_I2C_BUS=0` (for oldest RPi1)
         *
         * @ingroup arduino-i2c
         */
        #define PINNACLE_DEFAULT_I2C_BUS 1
    #endif

/**
 * Specific exception for I2C errors
 *
 * @ingroup arduino-i2c
 */
class I2CException : public std::runtime_error
{
public:
    explicit I2CException(const std::string& msg)
        : std::runtime_error(msg)
    {
    }
};

/**
 * A class to wrap platform-specific implementation of the I2C data bus in Arduino-like API.
 *
 * .. failure:: Missing features
 *
 *     This implementation does not support slave device behavior or interrupt service routines.
 *
 * @ingroup arduino-i2c
 */
class TwoWire
{
private:
    uint8_t slaveAddress;
    static uint8_t* txBuff;
    uint8_t txBuffIndex;
    uint8_t txBuffLen;
    static uint8_t* rxBuff;
    uint8_t rxBuffIndex;
    uint8_t rxBuffLen;

public:
    /** Instantiate the object for use with the I2C bus. */
    TwoWire();

    /** Initialize the I2C bus' pins.
     *
     * @param busNumber The I2C bus number as identified by the directory listing in
     *     :literal:`/dev/i2c-*`. For :literal:`/dev/i2c-1`, this parameter’s value should be
     *     ``1``. Defaults to ``1``, but the first Raspberry Pi board uses :literal:`/dev/i2c-0`.
     *
     *     .. info:: Difference with ``mraa`` driver
     *
     *         If using the ``mraa`` driver, then this number is not guaranteed to coincide with the
     *         actual I2C bus number (:literal:`/dev/i2c-<x>`). See the `MRAA source code
     *         <https://github.com/eclipse/mraa/tree/master/src>`_ for your platform to determine
     *         what number to use for which I2C bus.
     *
     *         For compatibility reasons, this parameter defaults to ``0`` when using the
     *         ``mraa`` driver.
     */
    void begin(uint8_t busNumber = PINNACLE_DEFAULT_I2C_BUS);

    /** De-initialize the I2C bus' pins. */
    void end();

    /**
     * Start a transaction over the bus.
     *
     * .. note:: This will reset the internal buffer used for `write()`.
     *
     * @param address The slave device's I2C address.
     */
    void beginTransmission(uint8_t address);

    /**
     * Terminate transactions on the bus.
     *
     * This function is used to send the bytes in the internal buffer (set with `write()`) over
     * the bus with an optional stop condition.
     *
     * .. note:: This will also reset the internal buffer.
     *
     * @param sendStop If set to a non-zero value, then a stop condition is sent over the bus.
     *
     *     .. task-list::
     *         :class: custom-task-list-style
     *
     *         - [ ] This parameter is ignored on Linux implementations.
     *
     * @return The number of bytes sent over the bus.
     */
    uint8_t endTransmission(uint8_t sendStop = false);

    /**
     * Write a single byte to the bus.
     *
     * .. note::
     *     This must be called after `beginTransmission()`. After all bytes are ready to be sent,
     *     use `endTransmission()` to send the internal buffer over the bus.
     *
     * @param data The single byte to add to the internal buffer.
     *
     * @returns The number of bytes written to the internal buffer.
     *
     *     .. warning::
     *         This implementation uses an internal buffer allocated with ``32`` bytes.
     *         If the internal buffer's maximum capacity is reached, then this will return ``0``.
     */
    size_t write(uint8_t data);

    /**
     * Request a number of bytes to read from the specified I2C device's address.
     *
     * @param address The slave device's I2C address. To specify the register offset, use
     *     `beginTransmission()` and `write()` the register offset.
     * @param quantity The number of bytes to request.
     *
     *     .. warning::
     *         This implementation uses an internal buffer allocated with ``32`` bytes.
     *         If requesting more than 32 bytes, then this function will only read up to 32 bytes.
     * @param sendStop If set to a non-zero value, then a stop condition is sent over the bus
     *     after the transaction is completed.
     *
     *     .. task-list::
     *         :class: custom-task-list-style
     *
     *         - [ ] This parameter is ignored on Linux implementations.
     *
     * @return The number of bytes read into the internal buffer.
     */
    uint8_t requestFrom(uint8_t address, uint8_t quantity, uint8_t sendStop);

    /**
     * Check for remaining bytes in the internal buffer.
     *
     * .. note::
     *     This must be called after `requestFrom()` in order to capture bytes from the bus into
     *     the internal RX buffer.
     *
     * @returns The number of bytes left in the buffer. A negative number (or zero) means
     *     all data from buffer has been `read()`.
     */
    int available();

    /**
     * Read a byte from the internal buffer.
     *
     * @returns The next single byte from the internal buffer.
     *     Use `available()` to prevent out of bounds and/or segmentation fault errors.
     */
    int read();

    /* free up the allocated memory for internal buffers
    virtual ~TwoWire(); */
};

/**
 * Pre-instantiated I2C bus object (to use as a convenient default).
 *
 * @ingroup arduino-i2c
 */
extern TwoWire Wire;

} // namespace cirque_pinnacle_arduino_wrappers

#endif // !defined(ARDUINO)
#endif // CIRQUEPINNACLE_UTILITY_TEMPLATE_I2C_H_
