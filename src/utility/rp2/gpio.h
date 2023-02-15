#ifndef CIRQUEPINNACLE_UTILITY_RP2_GPIO_H_
#define CIRQUEPINNACLE_UTILITY_RP2_GPIO_H_
#ifndef ARDUINO

    #include "pico/stdlib.h"

typedef uint8_t pinnacle_gpio_t;

namespace cirque_pinnacle_arduino_wrappers {

class GPIOClass
{

public:
    static const bool DIRECTION_OUT = true;
    static const bool DIRECTION_IN = false;

    static const bool OUTPUT_HIGH = true;
    static const bool OUTPUT_LOW = false;

    GPIOClass();

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
    static void write(pinnacle_gpio_t port, bool value);

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
#endif // CIRQUEPINNACLE_UTILITY_RP2_GPIO_H_
