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
#ifndef ARDUINO
    #include <map>
    #include <mraa.h>   // mraa_strresult()
    #include <mraa.hpp> // mraa::Gpio
    #include "interrupt.h"
    #include "gpio.h" // pinnacle_gpio_t

    #ifdef __cplusplus
extern "C" {
    #endif

namespace cirque_pinnacle_arduino_wrappers {

    std::map<pinnacle_gpio_t, mraa::Gpio*> irqCache;

    int attachInterrupt(pinnacle_gpio_t pin, uint8_t mode, void (*function)(void))
    {
        // ensure pin is not already being used in a separate thread
        detachInterrupt(pin);
        GPIOClass::close(pin);

        mraa::Gpio* gpio = new mraa::Gpio(pin);
        mraa::Result status = gpio->dir(mraa::DIR_IN);
        if (status != mraa::SUCCESS) {
            std::string msg = "[attachInterrupt] Could not set the pin as an input; ";
            msg += mraa_strresult((mraa_result_t)status);
            throw IRQException(msg);
            return 0;
        }
        status = gpio->isr((mraa::Edge)mode, (void (*)(void*))function, NULL);
        if (status != mraa::SUCCESS) {
            std::string msg = "[attachInterrupt] Could not setup the ISR; ";
            msg += mraa_strresult((mraa_result_t)status);
            throw IRQException(msg);
            return 0;
        }

        std::pair<std::map<pinnacle_gpio_t, mraa::Gpio*>::iterator, bool> indexPair = irqCache.insert(std::pair<pinnacle_gpio_t, mraa::Gpio*>(pin, gpio));
        if (!indexPair.second) {
            // this should not be reached, but indexPair.first needs to be the inserted map element
            gpio->close();
            throw IRQException("[attachInterrupt] Could not cache the mraa::Gpio object");
            return 0;
        }
        return 1;
    }

    int detachInterrupt(pinnacle_gpio_t pin)
    {
        std::map<pinnacle_gpio_t, mraa::Gpio*>::iterator cachedPin = irqCache.find(pin);
        if (cachedPin == irqCache.end()) {
            return 0; // pin not in cache; just exit
        }
        cachedPin->second->close();
        irqCache.erase(cachedPin);
        return 1;
    }

} // namespace cirque_pinnacle_arduino_wrappers

    #ifdef __cplusplus
}
    #endif

#endif // !defined(ARDUINO)
