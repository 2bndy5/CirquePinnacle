#ifndef CIRQUEPINNACLE_UTILITY_PIGPIO_SPI_H_
#define CIRQUEPINNACLE_UTILITY_PIGPIO_SPI_H_
#ifndef ARDUINO

    #include <cstdint>
    #include <stdexcept>

namespace cirque_pinnacle_arduino_wrappers {

    #ifndef PINNACLE_SPI_SPEED
        // Default is the maximum supported SPI speed (13 MHz)
        #define PINNACLE_SPI_SPEED 13000000
    #endif

    #ifndef PINNACLE_DEFAULT_SPI_BUS
        // Default is RPi's CE0 pin on the primary bus 0: /dev/spidev0.0
        #define PINNACLE_DEFAULT_SPI_BUS 0
    #endif

    #define PINNACLE_SS_CTRL(pin, value)

/** Specific exception for SPI errors */
class SPIException : public std::runtime_error
{
public:
    explicit SPIException(const std::string& msg)
        : std::runtime_error(msg)
    {
    }
};

class SPIClass
{

public:
    /** Instantiate an object for use with a single SPi bus. */
    SPIClass();

    /**
     * Initialize a certain SPI bus. Uses boards defaults when available.
     * @param busNumber This is the SPI bus number and corresponding channel (CEx pin on RPi).
     * | bus ID | CE number | param value | spidev device  |
     * |:------:|:---------:|:-----------:|:--------------:|
     * |    0   |     0     |      0      | /dev/spidev0.0 |
     * |    0   |     1     |      1      | /dev/spidev0.1 |
     * |    1   |     0     |     10      | /dev/spidev1.0 |
     * |    1   |     1     |     11      | /dev/spidev1.1 |
     * |    1   |     2     |     12      | /dev/spidev1.2 |
     * @param spi_speed The baudrate (aka frequency) to be used on the specified SPI bus.
     */
    void begin(uint8_t busNumber = PINNACLE_DEFAULT_SPI_BUS, uint32_t spi_speed = PINNACLE_SPI_SPEED);

    /**
     * Transfer buffers of bytes to/from a SPI slave device.
     * @param tx_buf The pointer to a buffer of bytes to send over MOSI.
     * @param rx_buf The pointer to a buffer of bytes that get received over MISO.
     * @param len The length of each buffer of bytes; each buffer should have equal length.
     */
    void transfernb(char* tx_buf, char* rx_buf, uint32_t len);

    /**
     * Transfer a buffer of bytes to a SPI slave device.
     * @param buf The pointer to a buffer of bytes to send over MOSI.
     * @param len The length of the buffer of bytes.
     * 
     * @note The bytes received over MISO will replace the 
     * buffer contents as the bytes are sent over MOSI.
     */
    void transfern(char* buf, uint32_t len);

    /**
     * Transfer a buffer of bytes to a SPI slave device.
     * @param buf The pointer to a buffer of bytes to send over MOSI.
     * @param len The length of the buffer of bytes.
     * 
     * @note The bytes received over MISO will replace the 
     * buffer contents as the bytes are sent over MOSI.
     */
    uint8_t transfer(char tx);

    /** Clean-up any internal pointers/buffers/etc. */
    ~SPIClass();

private:
    unsigned spiHandle;
    bool spiIsInitialized = false;
    void init(uint32_t spi_speed);
};

// pre-instantiated SPI bus object (to use as a convenient default)
extern SPIClass SPI;

} // namespace cirque_pinnacle_arduino_wrappers

#endif // !defined(ARDUINO)
#endif // CIRQUEPINNACLE_UTILITY_PIGPIO_SPI_H_
