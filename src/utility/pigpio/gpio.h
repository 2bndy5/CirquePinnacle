#ifndef CIRQUEPINNACLE_UTILITY_PIGPIO_GPIO_H_
#define CIRQUEPINNACLE_UTILITY_PIGPIO_GPIO_H_
#ifndef ARDUINO

    #include <cstdio>
    #include <map>
    #include <stdexcept>

typedef int pinnacle_gpio_t;

namespace cirque_pinnacle_arduino_wrappers {

/** Specific exception for GPIO errors */
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
    /* Constants */
    static const int DIRECTION_OUT = 1;
    static const int DIRECTION_IN = 0;

    static const int OUTPUT_HIGH = 1;
    static const int OUTPUT_LOW = 0;

    GPIOClass();

    /**
     * Similar to Arduino pinMode(pin, mode);
     * @param port
     * @param direction
     */
    static void open(pinnacle_gpio_t port, int direction);

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

    virtual ~GPIOClass();
};

    #define INPUT                    GPIOClass::DIRECTION_IN
    #define OUTPUT                   GPIOClass::DIRECTION_OUT
    #define HIGH                     GPIOClass::OUTPUT_HIGH
    #define LOW                      GPIOClass::OUTPUT_LOW
    #define digitalRead(pin)         GPIOClass::read(pin)
    #define digitalWrite(pin, value) GPIOClass::write(pin, value)
    #define pinMode(pin, direction)  GPIOClass::open(pin, direction)

} // namespace cirque_pinnacle_arduino_wrappers

#endif // !defined(ARDUINO)
#endif // CIRQUEPINNACLE_UTILITY_PIGPIO_GPIO_H_
