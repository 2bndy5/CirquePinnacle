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
#ifndef CIRQUEPINNACLE_UTILITY_MRAA_INTERRUPT_H_
#define CIRQUEPINNACLE_UTILITY_MRAA_INTERRUPT_H_
#ifndef ARDUINO

    #include <stdexcept> // std::exception, std::string
    #include "mraa.hpp"  // mraa::
    #include "gpio.h"    // pinnacle_gpio_t

    #ifdef __cplusplus
extern "C" {
    #endif

namespace cirque_pinnacle_arduino_wrappers {

    enum Edge
    {
        FALLING = mraa::Edge::EDGE_FALLING,
        RISING = mraa::Edge::EDGE_RISING,
        CHANGE = mraa::Edge::EDGE_BOTH,
    };

    /** Specific exception for IRQ errors */
    class IRQException : public std::runtime_error
    {
    public:
        explicit IRQException(const std::string& msg)
            : std::runtime_error(msg)
        {
        }
    };

    /**
     * Take the details and create an interrupt handler that will
     * callback to the user-supplied function.
     */
    int attachInterrupt(pinnacle_gpio_t pin, uint8_t mode, void (*function)(void));

    /**
     * Will cancel the interrupt thread, close the filehandle and release the pin.
     */
    int detachInterrupt(pinnacle_gpio_t pin);

} // namespace cirque_pinnacle_arduino_wrappers

    #ifdef __cplusplus
}
    #endif

#endif // !defined(ARDUINO)
#endif // CIRQUEPINNACLE_UTILITY_MRAA_INTERRUPT_H_
