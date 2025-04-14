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
#ifndef CIRQUEPINNACLE_UTILITY_LINUX_KERNEL_GPIO_H_
#define CIRQUEPINNACLE_UTILITY_LINUX_KERNEL_GPIO_H_
#ifndef ARDUINO

    #include <cstdint>      // uintXX_t
    #include <stdexcept>    // std::exception, std::string
    #include "linux/gpio.h" // gpiochip_info

    #ifdef __cplusplus
extern "C" {
    #endif

typedef unsigned int pinnacle_gpio_t;

    #ifndef PINNACLE_LINUX_GPIO_CHIP
        /**
         * The default GPIO chip to use.  Defaults to `/dev/gpiochip0`.
         */
        #define PINNACLE_LINUX_GPIO_CHIP "/dev/gpiochip0"
    #endif
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

    /// A struct to manage the GPIO chip file descriptor.
    /// This struct's destructor should close any cached GPIO pin requests' file descriptors.
    struct GPIOChipCache
    {
        static int fd;

        /// Open the File Descriptor for the GPIO chip
        void openDevice();

        /// Close the File Descriptor for the GPIO chip
        void closeDevice();

        /// should be called automatically on program start.
        /// Here, we do some one-off configuration.
        GPIOChipCache();

        /// Should be called automatically on program exit.
        /// What we need here is to make sure that the File Descriptors used to
        /// control GPIO pins are properly closed.
        ~GPIOChipCache();
    };

    class GPIOClass
    {

    public:
        GPIOClass();

        static const bool DIRECTION_OUT = true;
        static const bool DIRECTION_IN = false;

        static const bool OUTPUT_HIGH = true;
        static const bool OUTPUT_LOW = false;

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

    #ifdef __cplusplus
}
    #endif

#endif // !defined(ARDUINO)
#endif // CIRQUEPINNACLE_UTILITY_LINUX_KERNEL_GPIO_H_
