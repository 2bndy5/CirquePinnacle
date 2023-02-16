#ifndef CIRQUEPINNACLE_UTILITY_MRAA_SPI_H_
#define CIRQUEPINNACLE_UTILITY_MRAA_SPI_H_
#ifndef ARDUINO

    #include "mraa.h"
    #include "mraa.hpp"

namespace cirque_pinnacle_arduino_wrappers {

    #ifndef PINNACLE_SPI_SPEED
        // Default is the maximum supported SPI speed (13 MHz)
        #define PINNACLE_SPI_SPEED 13000000
    #endif

    #ifndef PINNACLE_DEFAULT_SPI_BUS
        // Default is RPi's CE0 pin on the primary bus 0: /dev/spidev0.0
        #define PINNACLE_DEFAULT_SPI_BUS 0
    #endif

    #define MSBFIRST  1
    #define LSBFIRST  0
    #define SPI_MODE0 mraa::SPI_MODE0
    #define SPI_MODE1 mraa::SPI_MODE1
    #define SPI_MODE2 mraa::SPI_MODE2
    #define SPI_MODE3 mraa::SPI_MODE3

    // this SPI implements beginTransaction() & endTransaction() to
    // configure the SPI bus
    #define SPI_HAS_TRANSACTION 1

    #define PINNACLE_SS_CTRL(pin, value)

class SPISettings
{

public:
    SPISettings(uint32_t clock, uint8_t bitOrder, mraa::Spi_Mode dataMode)
    {
        init(clock, bitOrder, dataMode);
    }

    SPISettings()
    {
        init(PINNACLE_SPI_SPEED, MSBFIRST, SPI_MODE1);
    }

    uint32_t clock;
    uint8_t bitOrder;
    mraa::Spi_Mode mode;

private:
    void init(uint32_t _clock, uint8_t _bitOrder, mraa::Spi_Mode _dataMode)
    {
        clock = _clock;
        bitOrder = _bitOrder;
        mode = _dataMode;
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
     */
    void begin(int busNumber = PINNACLE_DEFAULT_SPI_BUS);

    /** Deinitialize the SPI bus. */
    void end();

    /**
     * Configure the SPI bus.
     * @param spiSettings A `SPISettings` object that encapsulates SPI bus configuration.
     */
    void beginTransaction(SPISettings spiSettings);

    /** a non-op to allow using beginTransaction() */
    void endTransaction();

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
     * Transfer a single byte to/from a SPI slave device.
     * @param tx The byte to send over MOSI.
     * @return The byte received over MISO when sending the `tx` byte over MOSI.
     */
    uint8_t transfer(uint8_t tx);

    /** Clean-up any internal pointers/buffers/etc. */
    virtual ~SPIClass();

private:
    mraa::Spi* spi_inst;
};

// declare a instantiated onject (for use as a convenient default)
extern SPIClass SPI;

} // namespace cirque_pinnacle_arduino_wrappers

#endif // !defined(ARDUINO)
#endif // CIRQUEPINNACLE_UTILITY_MRAA_SPI_H_
