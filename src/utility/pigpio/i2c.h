#ifndef CIRQUEPINNACLE_UTILITY_PIGPIO_I2C_H_
#define CIRQUEPINNACLE_UTILITY_PIGPIO_I2C_H_
#ifndef ARDUINO

    #include <cstdint>
    #include <stdexcept>

    #define I2C_BUS0 0
    #define I2C_BUS1 1

    #ifndef PINNACLE_DEFAULT_I2C_BUS
        // Convenient alias to control which I2C bus is used by default.
        // Via compiler flag: `-D PINNACLE_DEFAULT_I2C_BUS=0` (for oldest RPi1)
        #define PINNACLE_DEFAULT_I2C_BUS I2C_BUS1
    #endif

namespace cirque_pinnacle_arduino_wrappers {

/** Specific exception for I2C errors */
class I2CException : public std::runtime_error
{
public:
    explicit I2CException(const std::string& msg)
        : std::runtime_error(msg)
    {
    }
};

class TwoWire
{

public:
    /** Instantiate the object for use with the I2C bus. */
    TwoWire();

    /** Initialize the I2C bus' pins. */
    void begin(uint8_t bus_number = PINNACLE_DEFAULT_I2C_BUS, uint8_t slaveAddress = 0x2A);

    /** De-initialize the I2C bus' pins. */
    void end();

    /**
     * Start a transaction over the bus.
     * @note This will reset the internal TX buffer used for write().
     * @param address The slave device's I2C address.
     */
    void beginTransmission(uint8_t address);

    /**
     * Terminate transactions on the bus.
     * 
     * This function is used to send the bytes in the internal TX buffer (set with `write()`) over
     * the bus with an optional stop condition.
     * 
     * @note This will also reset the internal TX buffer.
     * @param sendStop If set to a non-zero value, then a stop condition is sent over the bus.
     * @return The number of bytes sent over the bus.
     */
    uint8_t endTransmission(uint8_t sendStop = false);

    /**
     * Write a single byte to the bus.
     * @note This must be called after `beginTransmission()`. After all bytes are ready to be sent,
     * use `endTransmission()` to send the internal TX buffer over the bus.
     * @returns The number of bytes written to the internal TX buffer.
     */
    size_t write(uint8_t data);

    /**
     * Request a number of bytes to read from the specified I2C device's address.
     * @param address The slave device's I2C address.
     * To specify the register offset, use beginTransmission() and write() the register offset.
     * @param quantity The number of bytes to request.
     * @param sendStop If set to a non-zero value, then a stop condition is sent over the bus
     * after the transaction is completed.
     * @return The number of bytes read into the internal RX buffer.
     */
    uint8_t requestFrom(uint8_t address, uint8_t quantity, uint8_t sendStop);

    /**
     * Check for remaining bytes in the internal RX buffer.
     * @note This must be called after `requestFrom()` in order to capture bytes from the bus into
     * the internal RX buffer.
     * @return the number of bytes left in the RX buffer. A negative number (or zero) means
     * all data from RX buffer has been `read()`.
     */
    int available();

    /**
     * Read a byte from the internal RX buffer.
     * @returns The next single byte from the internal RX buffer.
     * Use `available()` to prevent out of bounds and/or segmentation fault errors.
     */
    int read();

    /** free up the allocated memory for internal buffers */
    virtual ~TwoWire();

private:
    uint8_t* xBuff;
    uint8_t xBuffIndex;
    uint8_t xBuffLen;

    int devHandle;
    uint8_t slaveAddress;
    bool i2cInitialized;
    uint8_t busID;
};

// pre-instantiated I2C bus object (to use as a convenient default)
extern TwoWire Wire;

} // namespace cirque_pinnacle_arduino_wrappers

#endif // !defined(ARDUINO)
#endif // CIRQUEPINNACLE_UTILITY_PIGPIO_I2C_H_