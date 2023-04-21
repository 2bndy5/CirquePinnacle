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
#ifndef CIRQUEPINNACLE_UTILITY_BCM2XXX_GPIO_H_
#define CIRQUEPINNACLE_UTILITY_BCM2XXX_GPIO_H_
#ifndef ARDUINO

    #include "bcm2835.h"

    #ifdef __cplusplus
extern "C" {
    #endif

typedef uint8_t pinnacle_gpio_t;
const pinnacle_gpio_t PINNACLE_SW_DR = 0xFF;

namespace cirque_pinnacle_arduino_wrappers {

    // include this header in other /utility/bcm2xxx headers to init the lib globally.
    static bool bcmInitialized = (bool)bcm2835_init();

    #define INPUT  BCM2835_GPIO_FSEL_INPT
    #define OUTPUT BCM2835_GPIO_FSEL_OUTP
    // HIGH and LOW are declared in bcm2835.h
    #define digitalRead(pin)         bcm2835_gpio_lev(pin)
    #define digitalWrite(pin, value) bcm2835_gpio_write(pin, value)
    #define pinMode(pin, value)      bcm2835_gpio_fsel(pin, value)

} // namespace cirque_pinnacle_arduino_wrappers

    #ifdef __cplusplus
}
    #endif

#endif // !defined(ARDUINO)
#endif // CIRQUEPINNACLE_UTILITY_BCM2XXX_GPIO_H_
