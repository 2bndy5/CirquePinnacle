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
#ifndef CIRQUEPINNACLE_UTILITY_TEMPLATE_GPIO_H
#define CIRQUEPINNACLE_UTILITY_TEMPLATE_GPIO_H
#ifndef ARDUINO

    #include <stdexcept>

/**
 * An alias for platform-specific representation of GPIO pin numbers
 *
 * @ingroup arduino-gpio
 */
typedef uint16_t pinnacle_gpio_t;

namespace cirque_pinnacle_arduino_wrappers {

/**
 * Specific exception for GPIO errors
 *
 * @ingroup arduino-gpio
 */
class GPIOException : public std::runtime_error
{
public:
    explicit GPIOException(const std::string& msg)
        : std::runtime_error(msg)
    {
    }
};

/**
 * A class to wrap platform-specific implementation of GPIO pins in Arduino-like API.
 *
 * @ingroup arduino-gpio
 */
class GPIOClass
{

public:
    static const int DIRECTION_OUT = 1;
    static const int DIRECTION_IN = 0;

    static const int OUTPUT_HIGH = 1;
    static const int OUTPUT_LOW = 0;

    GPIOClass();

    /**
     * Similar to Arduino ``pinMode(pin, mode)``.
     *
     * @param port The GPIO pin.
     * @param direction The direction of the signals (``INPUT`` or ``OUTPUT``).
     */
    static void open(pinnacle_gpio_t port, int direction);

    /**
     * Defined for completeness; not actually exposed in the Arduino API
     *
     * @param port The GPIO pin.
     */
    static void close(pinnacle_gpio_t port);

    /**
     * Similar to Arduino ``digitalRead(pin)``.
     *
     * @param port The GPIO pin.
     */
    static int read(pinnacle_gpio_t port);

    /**
     * Similar to Arduino ``digitalWrite(pin, level)``.
     *
     * @param port The GPIO pin.
     * @param value The binary level of an output signal (``HIGH`` or ``LOW``).
     */
    static void write(pinnacle_gpio_t port, int value);

    /* virtual ~GPIOClass(); */

private:
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
#endif // CIRQUEPINNACLE_UTILITY_TEMPLATE_GPIO_H
