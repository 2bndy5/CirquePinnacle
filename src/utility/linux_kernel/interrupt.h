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
#ifndef CIRQUEPINNACLE_UTILITY_LINUX_KERNEL_INTERRUPT_H_
#define CIRQUEPINNACLE_UTILITY_LINUX_KERNEL_INTERRUPT_H_
#ifndef ARDUINO

    #include <pthread.h> // pthread_t
    #include <stdexcept> // std::exception, std::string
    #include "gpio.h"    // pinnacle_gpio_t

    #ifdef __cplusplus
extern "C" {
    #endif

namespace cirque_pinnacle_arduino_wrappers {

    enum Edge
    {
        FALLING = GPIO_V2_LINE_FLAG_EDGE_FALLING,
        RISING = GPIO_V2_LINE_FLAG_EDGE_RISING,
        CHANGE = GPIO_V2_LINE_FLAG_EDGE_FALLING | GPIO_V2_LINE_FLAG_EDGE_RISING,
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

    /** Details related to a certain pin's ISR. */
    struct IrqPinCache
    {
        /// The pin request's file descriptor
        int fd = 0;

        /// The posix thread ID.
        pthread_t id = 0;

        /// The user-designated ISR function (used as a callback)
        void (*function)(void) = nullptr;
    };

    /**
     * Take the details and create an interrupt handler that will
     * callback to the user-supplied function.
     */
    int attachInterrupt(pinnacle_gpio_t pin, unsigned long long mode, void (*function)(void));

    /**
     * Will cancel the interrupt thread, close the filehandle and release the pin.
     */
    int detachInterrupt(pinnacle_gpio_t pin);

} // namespace cirque_pinnacle_arduino_wrappers

    #ifdef __cplusplus
}
    #endif

#endif // !defined(ARDUINO)
#endif // CIRQUEPINNACLE_UTILITY_LINUX_KERNEL_INTERRUPT_H_
