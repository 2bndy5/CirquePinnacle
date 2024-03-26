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
    #include <pthread.h>
    #include <map>
    #include "bcm2835.h"
    #include "interrupt.h"

    #ifdef __cplusplus
extern "C" {
    #endif

namespace cirque_pinnacle_arduino_wrappers {

    static pthread_mutex_t irq_mutex = PTHREAD_MUTEX_INITIALIZER;
    std::map<pinnacle_gpio_t, IrqPinCache> irqCache;

    void* poll_irq(void* arg)
    {
        IrqPinCache* pinCache = (IrqPinCache*)(arg);

        for (;;) {
            int ret = bcm2835_gpio_eds(pinCache->pin);
            if (ret > 0) {
                bcm2835_gpio_set_eds(pinCache->pin);
                pinCache->function();
            }
            pthread_testcancel();
        }
        return NULL;
    }

    int attachInterrupt(pinnacle_gpio_t pin, uint8_t mode, void (*function)(void))
    {
        // ensure pin is not already being used in a separate thread
        detachInterrupt(pin);

        // create a request object to configure the specified pin
        switch (mode) {
            case CHANGE:
                bcm2835_gpio_ren(pin);
                bcm2835_gpio_fen(pin);
                break;
            case RISING:
                bcm2835_gpio_ren(pin);
                break;
            case FALLING:
                bcm2835_gpio_fen(pin);
                break;
            default:
                // bad user input!
                return 0; // stop here
        }

        // cache details
        IrqPinCache irqPinCache;
        irqPinCache.pin = pin;
        irqPinCache.function = function;
        std::pair<std::map<pinnacle_gpio_t, IrqPinCache>::iterator, bool> indexPair = irqCache.insert(std::pair<pinnacle_gpio_t, IrqPinCache>(pin, irqPinCache));

        if (!indexPair.second) {
            // this should not be reached, but indexPair.first needs to be the inserted map element
            throw IRQException("[attachInterrupt] Could not cache the IRQ pin with function pointer");
            return 0;
        }

        // create and start thread
        pthread_mutex_lock(&irq_mutex);
        pthread_create(&indexPair.first->second.id, nullptr, poll_irq, &indexPair.first->second);
        pthread_mutex_unlock(&irq_mutex);

        return 1;
    }

    int detachInterrupt(pinnacle_gpio_t pin)
    {
        std::map<pinnacle_gpio_t, IrqPinCache>::iterator cachedPin = irqCache.find(pin);
        if (cachedPin == irqCache.end()) {
            return 0; // pin not in cache; just exit
        }
        pthread_cancel(cachedPin->second.id);     // send cancel request
        pthread_join(cachedPin->second.id, NULL); // wait till thread terminates
        bcm2835_gpio_clr_ren(cachedPin->second.pin);
        bcm2835_gpio_clr_fen(cachedPin->second.pin);
        bcm2835_gpio_set_eds(cachedPin->second.pin);
        irqCache.erase(cachedPin);
        return 1;
    }

} // namespace cirque_pinnacle_arduino_wrappers

    #ifdef __cplusplus
}
    #endif

#endif // !defined(ARDUINO)
