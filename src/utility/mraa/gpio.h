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
#ifndef CIRQUEPINNACLE_UTILITY_MRAA_GPIO_H_
#define CIRQUEPINNACLE_UTILITY_MRAA_GPIO_H_
#ifndef ARDUINO

    #include <stdexcept>
    #include <map>
    #include "mraa.hpp"

    #ifdef __cplusplus
extern "C" {
    #endif

typedef int pinnacle_gpio_t;
const pinnacle_gpio_t PINNACLE_SW_DR = 0x7FFFFFFF;

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
        static std::map<pinnacle_gpio_t, mraa::Gpio*> cache;
    };

    #define INPUT                    mraa::DIR_IN
    #define OUTPUT                   mraa::DIR_OUT
    #define HIGH                     GPIOClass::OUTPUT_HIGH
    #define LOW                      GPIOClass::OUTPUT_LOW
    #define digitalWrite(pin, value) GPIOClass::write(pin, value)
    #define digitalRead(pin)         GPIOClass::read(pin)
    #define pinMode(pin, direction)  GPIOClass::open(pin, direction)

} // namespace cirque_pinnacle_arduino_wrappers

    #ifdef __cplusplus
}
    #endif

#endif // !defined(ARDUINO)
#endif // CIRQUEPINNACLE_UTILITY_MRAA_GPIO_H_
