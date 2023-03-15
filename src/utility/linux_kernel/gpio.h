
#ifndef CIRQUEPINNACLE_UTILITY_LINUX_KERNEL_GPIO_H_
#define CIRQUEPINNACLE_UTILITY_LINUX_KERNEL_GPIO_H_
#ifndef ARDUINO

    #include <stdexcept>
    #include <map>

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

typedef int GPIOfdCache_t;

class GPIOClass
{

public:
    GPIOClass();

    static constexpr char OUTPUT_HIGH[2] = {'1', '\n'};
    static constexpr char OUTPUT_LOW[2] = {'0', '\n'};
    static const bool DIRECTION_IN = false;
    static const bool DIRECTION_OUT = true;

    /**
     * Similar to Arduino pinMode(pin, mode);
     * @param port
     * @param direction
     */
    static void open(pinnacle_gpio_t port, bool direction);

    /**
     * Defined for completeness; not actually exposed in the Arduino API
     * @param port
     */
    static void close(pinnacle_gpio_t port);

    /**
     * Similar to Arduino digitalRead(pin);
     * @param port
     */
    static bool read(pinnacle_gpio_t port);

    /**
    * Similar to Arduino digitalWrite(pin, level);
    * @param port
    * @param value
    */
    static void write(pinnacle_gpio_t port, const char* value);

    virtual ~GPIOClass();

private:
    /* fd cache */
    static std::map<pinnacle_gpio_t, GPIOfdCache_t> cache;
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
#endif // CIRQUEPINNACLE_UTILITY_LINUX_KERNEL_GPIO_H_
