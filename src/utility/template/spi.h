/*
 * Copyright (c) 2025 Brendan Doherty (2bndy5)
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
#ifndef CIRQUEPINNACLE_UTILITY_TEMPLATE_SPI_H_
#define CIRQUEPINNACLE_UTILITY_TEMPLATE_SPI_H_
#ifndef ARDUINO

    #include <cstdint>
    #include <stdexcept>

    // exclude C linkage for CPP docs generation
    #if defined(__cplusplus) && !defined(CIRQUEPINNACLE_UTILITY_TEMPLATE_SPI_H_)
extern "C" {
    #endif

namespace cirque_pinnacle_arduino_wrappers {

    #ifndef PINNACLE_SPI_SPEED
        /**
         * Default is the recommended 6 MHz (maximum supported SPI speed is 13 MHz)
         *
         * @ingroup arduino-spi
         */
        #define PINNACLE_SPI_SPEED 6000000
    #endif

    #ifndef PINNACLE_DEFAULT_SPI_BUS
        /// @ingroup arduino-spi
        #define PINNACLE_DEFAULT_SPI_BUS 0
    #endif

    /// Uses ``digitalWrite()`` when not using SPI bus' native CS pin
    #define PINNACLE_SS_CTRL(pin, value)

    /**
     * Specific exception for SPI errors
     *
     * @ingroup arduino-spi
     */
    class SPIException : public std::runtime_error
    {
    public:
        explicit SPIException(const std::string& msg)
            : std::runtime_error(msg)
        {
        }
    };

    /**
     * An enumeration of endianess options.
     *
     * @ingroup arduino-spi
     */
    enum BitOrder : uint8_t
    {
        MSBFIRST = 0, ///< Most Significant Byte First
        LSBFIRST = 1  ///< Least Significant Byte First
    };

    /**
     * An enumeration of Clock interpretations.
     *
     * @ingroup arduino-spi
     */
    enum DataMode : uint8_t
    {
        /**
         * .. csv-table::
         *     :header: "Clock Polarity (CPOL)","Clock Phase (CPHA)","Output Edge","Capture Edge"
         *
         *     0,0,Falling,Rising
         */
        SPI_MODE0 = 0,
        /**
         * .. csv-table::
         *     :header: "Clock Polarity (CPOL)","Clock Phase (CPHA)","Output Edge","Capture Edge"
         *
         *     0,1,Rising,Falling
         */
        SPI_MODE1 = 1,
        /**
         * .. csv-table::
         *     :header: "Clock Polarity (CPOL)","Clock Phase (CPHA)","Output Edge","Capture Edge"
         *
         *     1,0,Rising,Falling
         */
        SPI_MODE2 = 2,
        /**
         * .. csv-table::
         *     :header: "Clock Polarity (CPOL)","Clock Phase (CPHA)","Output Edge","Capture Edge"
         *
         *     1,1,Falling,Rising
         */
        SPI_MODE3 = 3,
    };

    /**
     * A class to represent common SPI bus configurations
     *
     * @ingroup arduino-spi
     */
    class SPISettings
    {
    public:
        /**
         * @param clock The SPI speed (in Hz) to be used.
         * @param bitOrder The Endianess to be used. Options are:
         *
         *     - `MSBFIRST`
         *     - `LSBFIRST`
         * @param dataMode The clock phase and polarity to be used. Options are:
         *
         *     +---------------+-----------------------+--------------------+-------------+--------------+
         *     | ``dataMode``  | Clock Polarity (CPOL) | Clock Phase (CPHA) | Output Edge | Capture Edge |
         *     +===============+=======================+====================+=============+==============+
         *     |  `SPI_MODE0`  | 0                     | 0                  | Falling     | Rising       |
         *     +---------------+-----------------------+--------------------+-------------+--------------+
         *     |  `SPI_MODE1`  | 0                     | 1                  | Rising      | Falling      |
         *     +---------------+-----------------------+--------------------+-------------+--------------+
         *     |  `SPI_MODE2`  | 1                     | 0                  | Rising      | Falling      |
         *     +---------------+-----------------------+--------------------+-------------+--------------+
         *     |  `SPI_MODE3`  | 1                     | 1                  | Falling     | Rising       |
         *     +---------------+-----------------------+--------------------+-------------+--------------+
         *
         * @id custom
         */
        SPISettings(uint32_t clock, BitOrder bitOrder, DataMode dataMode)
        {
            init(clock, bitOrder, dataMode);
        }
        /**
         * A blank constructor to use the default configuration.
         *
         * The default configuration is 6 MHz speed (:expr:`6000000`), using `MSBFIRST`, and `SPI_MODE1`.
         *
         * @id default
         */
        SPISettings()
        {
            init(PINNACLE_SPI_SPEED, MSBFIRST, SPI_MODE1);
        }

        uint32_t clock;    ///< The SPI bus clock speed (in Hz).
        BitOrder bitOrder; ///< The SPI bus byte order (endianess).
        DataMode mode;     ///< The SPI bus mode (clock polarity/phase).

    private:
        void init(uint32_t _clock, BitOrder _bitOrder, DataMode _dataMode)
        {
            clock = _clock;
            bitOrder = _bitOrder;
            mode = _dataMode;
        }
    };

    /**
     * A class to wrap platform-specific implementation of the SPI data bus in Arduino-like API.
     *
     * .. failure:: Missing features
     *
     *     This implementation does not support slave device behavior or interrupt service routines.
     *
     * @ingroup arduino-spi
     */
    class SPIClass
    {
    public:
        /** Instantiate an object for use with a single SPi bus. */
        SPIClass();

        /**
         * Initialize a certain SPI bus. Uses boards defaults when available.
         *
         * @param bus_number This is the SPI bus number and corresponding channel (CEx pin on RPi).
         *     Default is ``0``.
         *
         *     .. default-literal-role::
         *
         *     +--------+-----------+-----------------+--------------------+
         *     | bus ID | CE number | param value     | spidev adapter     |
         *     +========+===========+=================+====================+
         *     |  ``0`` |   ``0``   |    :expr:`0`    | ``/dev/spidev0.0`` |
         *     +--------+-----------+-----------------+--------------------+
         *     |  ``0`` |   ``1``   |    :expr:`1`    | ``/dev/spidev0.1`` |
         *     +--------+-----------+-----------------+--------------------+
         *     |  ``1`` |   ``0``   |   :expr:`10`    | ``/dev/spidev1.0`` |
         *     +--------+-----------+-----------------+--------------------+
         *     |  ``1`` |   ``1``   |   :expr:`11`    | ``/dev/spidev1.1`` |
         *     +--------+-----------+-----------------+--------------------+
         *     |  ``1`` |   ``2``   |   :expr:`12`    | ``/dev/spidev1.2`` |
         *     +--------+-----------+-----------------+--------------------+
         *
         *     .. default-literal-role:: cpp
         * @param settings An object used to specify the baudrate (aka frequency), Endianess, and clock
         *     polarity/phase to be used on the specified SPI bus. Default is 6 MHz (``6000000``) speed
         *     using `MSBFIRST` (Big Endian) under `SPI_MODE1`.
         */
        void begin(int bus_number = PINNACLE_DEFAULT_SPI_BUS, SPISettings settings = SPISettings());

        /**
         * Transfer buffers of bytes to/from a SPI slave device.
         *
         * @param tx_buf The pointer to a buffer of bytes to send over MOSI.
         * @param rx_buf The pointer to a buffer of bytes that get received over MISO.
         * @param len The length of each buffer in bytes; each buffer must have equal length.
         *
         * @id buffered
         */
        void transfer(void* tx_buf, void* rx_buf, uint32_t len);

        /**
         * Transfer a buffer of bytes to a SPI slave device.
         *
         * @param buf The pointer to a buffer of bytes to send over MOSI.
         * @param len The length of the `buf` in bytes.
         *
         * .. note::
         *     The bytes received over MISO will replace the
         *     contents of `buf` as the bytes are sent over MOSI.
         *
         * @id bytes
         */
        void transfer(void* buf, uint32_t len);

        /**
         * Transfer a single byte to/from a SPI slave device.
         *
         * @param tx The byte to send over MOSI.
         *
         * @returns The byte received over MISO when sending the `tx` byte over MOSI.
         *
         * @id byte
         */
        uint8_t transfer(uint8_t tx);

        /* Clean-up any internal pointers/buffers/etc.
        virtual ~SPIClass(); */
    };

    /**
     * Pre-instantiated SPI bus object (to use as a convenient default).
     *
     * @ingroup arduino-spi
     */
    extern SPIClass SPI;

} // namespace cirque_pinnacle_arduino_wrappers

    // exclude C linkage for CPP docs generation
    #if defined(__cplusplus) && !defined(CIRQUEPINNACLE_UTILITY_TEMPLATE_SPI_H_)
}
    #endif

#endif // !defined(ARDUINO)
#endif // CIRQUEPINNACLE_UTILITY_TEMPLATE_SPI_H_
