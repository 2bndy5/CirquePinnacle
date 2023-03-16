/**
 * @file spi.h
 * @author TMRh20 2015
 * Class declaration for GPIO helper files
 */
#ifndef CIRQUEPINNACLE_UTILITY_MRAA_GPIO_H_
#define CIRQUEPINNACLE_UTILITY_MRAA_GPIO_H_
#ifndef ARDUINO

    #include <stdexcept>
    #include <map>
    #include "mraa.hpp"

typedef int pinnacle_gpio_t;

namespace cirque_pinnacle_arduino_wrappers {

/** Specific exception for SPI errors */
class GPIOException : public std::runtime_error
{
public:
    explicit GPIOException(const std::string& msg)
        : std::runtime_error(msg)
    {
    }
};

class GPIOClass
{

public:
    static const int OUTPUT_HIGH = 1;
    static const int OUTPUT_LOW = 0;

    GPIOClass();

    virtual ~GPIOClass();

    /**
     * Similar to Arduino pinMode(pin, mode);
     * @param port
     * @param direction
     */
    static void open(pinnacle_gpio_t port, mraa::Dir direction);

    /**
     * Defined for completeness; not actually exposed in the Arduino API
     * @param port
     */
    static void close(pinnacle_gpio_t port);

    /**
     * Similar to Arduino digitalRead(pin);
     * @param port
     */
    static int read(pinnacle_gpio_t port);

    /**
    * Similar to Arduino digitalWrite(pin, level);
    * @param port
    * @param value
    */
    static void write(pinnacle_gpio_t port, int value);

private:
    /* cache for mraa::Gpio instances */
    static std::map<pinnacle_gpio_t, mraa::Gpio> cache;
};

    #define INPUT                    mraa::DIR_IN
    #define OUTPUT                   mraa::DIR_OUT
    #define HIGH                     GPIOClass::OUTPUT_HIGH
    #define LOW                      GPIOClass::OUTPUT_LOW
    #define digitalWrite(pin, value) GPIOClass::write(pin, value)
    #define digitalRead(pin)         GPIOClass::read(pin)
    #define pinMode(pin, direction)  GPIOClass::open(pin, direction)

} // namespace cirque_pinnacle_arduino_wrappers

#endif // !defined(ARDUINO)
#endif // CIRQUEPINNACLE_UTILITY_MRAA_GPIO_H_
