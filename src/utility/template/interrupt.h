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
#ifndef CIRQUEPINNACLE_UTILITY_TEMPLATE_INTERRUPT_H_
#define CIRQUEPINNACLE_UTILITY_TEMPLATE_INTERRUPT_H_
#ifndef ARDUINO

    #include <stdexcept> // std::exception, std::string
    #include "gpio.h"    // pinnacle_gpio_t

    // exclude C linkage for CPP docs generation
    #if defined(__cplusplus) && !defined(CIRQUEPINNACLE_UTILITY_TEMPLATE_INTERRUPT_H_)
extern "C" {
    #endif

namespace cirque_pinnacle_arduino_wrappers {

    /**
     * The type of supported edge detection modes.
     *
     * @ingroup arduino-irq
     */
    enum Edge
    {
        /// An event in which the GPIO input changes from HIGH to LOW.
        FALLING,
        /// An event in which the GPIO input changes from LOW to HIGH.
        RISING,
        /// An event in which the GPIO input changes either from HIGH to LOW or HIGH to LOW.
        CHANGE,
    };

    /**
     * Specific exception for IRQ errors
     *
     * @ingroup arduino-irq
     */
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
     *
     * @param pin The number of the GPIO input pin to monitor for changes about edge detection.
     * @param mode The type of edge detection that invokes the specified callback ``function``.
     * @param function The user-defined function pointer that gets called when a specified event occurs.
     *
     * @ingroup arduino-irq
     */
    int attachInterrupt(pinnacle_gpio_t pin, int mode, void (*function)(void));

    /**
     * Will cancel the interrupt thread, close the filehandle and release the pin.
     *
     * @ingroup arduino-irq
     */
    int detachInterrupt(pinnacle_gpio_t pin);

} // namespace cirque_pinnacle_arduino_wrappers

    // exclude C linkage for CPP docs generation
    #if defined(__cplusplus) && !defined(CIRQUEPINNACLE_UTILITY_TEMPLATE_INTERRUPT_H_)
}
    #endif

#endif // !defined(ARDUINO)
#endif // CIRQUEPINNACLE_UTILITY_TEMPLATE_INTERRUPT_H_
