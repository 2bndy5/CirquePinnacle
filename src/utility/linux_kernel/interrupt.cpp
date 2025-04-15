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
#ifndef ARDUINO
    #include <unistd.h>    // close()
    #include <fcntl.h>     // open()
    #include <sys/ioctl.h> // ioctl()
    #include <errno.h>     // errno, strerror()
    #include <string.h>    // , strcpy()
    #include <pthread.h>
    #include <map>
    #include "linux/gpio.h"
    #include "interrupt.h"
    #include "gpio.h" // GPIOChipCache,

    #ifdef __cplusplus
extern "C" {
    #endif

namespace cirque_pinnacle_arduino_wrappers {

    static pthread_mutex_t irq_mutex = PTHREAD_MUTEX_INITIALIZER;
    std::map<pinnacle_gpio_t, IrqPinCache> irqCache;

    struct IrqChipCache : public GPIOChipCache
    {
        IrqChipCache() : GPIOChipCache() {};
        ~IrqChipCache()
        {
            for (std::map<pinnacle_gpio_t, IrqPinCache>::iterator i = irqCache.begin(); i != irqCache.end(); ++i) {
                pthread_cancel(i->second.id);     // send cancel request
                pthread_join(i->second.id, NULL); // wait till thread terminates
                // cached fd (i->second.fd) is closed by GPIOChipCache or GPIOClass::close()
            }
            irqCache.clear();
        }
    };

    IrqChipCache irqChipCache;

    void* poll_irq(void* arg)
    {
        IrqPinCache* pinCache = (IrqPinCache*)(arg);
        unsigned int lastEventSeqNo = 0;
        gpio_v2_line_event irqEventInfo;
        memset(&irqEventInfo, 0, sizeof(irqEventInfo));

        for (;;) {
            int ret = read(pinCache->fd, &irqEventInfo, sizeof(gpio_v2_line_event));
            if (ret < 0) {
                std::string msg = "[poll_irq] Could not read event info; ";
                msg += strerror(errno);
                throw IRQException(msg);
                return NULL;
            }
            if (ret > 0 && irqEventInfo.line_seqno != lastEventSeqNo) {
                lastEventSeqNo = irqEventInfo.line_seqno;
                pinCache->function();
            }
            pthread_testcancel();
        }
        return NULL;
    }

    bool attachInterrupt(pinnacle_gpio_t pin, void (*function)(void), unsigned long long mode)
    {
        // ensure pin is not already being used in a separate thread
        detachInterrupt(pin);
        GPIOClass::close(pin);

        try {
            irqChipCache.openDevice();
        }
        catch (GPIOException& exc) {
            if (irqChipCache.fd < 0) {
                irqChipCache.openDevice();
            }
        }

        // get chip info
        gpiochip_info info;
        memset(&info, 0, sizeof(info));
        int ret = ioctl(irqChipCache.fd, GPIO_GET_CHIPINFO_IOCTL, &info);
        if (ret < 0) {
            std::string msg = "[attachInterrupt] Could not gather info about ";
            msg += PINNACLE_LINUX_GPIO_CHIP;
            throw IRQException(msg);
            return 0;
        }

        if (pin > info.lines) {
            std::string msg = "[attachInterrupt] pin " + std::to_string(pin) + " is not available on " + PINNACLE_LINUX_GPIO_CHIP;
            throw IRQException(msg);
            return 0;
        }

        // create a request object to configure the specified pin
        struct gpio_v2_line_request request;
        memset(&request, 0, sizeof(request));
        strcpy(request.consumer, "RF24 IRQ");
        request.num_lines = 1U;
        request.offsets[0] = pin;
        request.event_buffer_size = sizeof(gpio_v2_line_event);

        // set debounce for the pin
        // request.config.attrs[0].mask = 1LL;
        // request.config.attrs[0].attr.id = GPIO_V2_LINE_ATTR_ID_DEBOUNCE;
        // request.config.attrs[0].attr.debounce_period_us = 10U;
        // request.config.num_attrs = 1U;

        // set pin as input and configure edge detection
        request.config.flags = GPIO_V2_LINE_FLAG_INPUT | GPIO_V2_LINE_FLAG_EVENT_CLOCK_REALTIME;
        switch (mode) {
            case CHANGE:
            case RISING:
            case FALLING:
                request.config.flags |= mode;
                break;
            default:
                // bad user input!
                return 0; // stop here
        }

        // write pin request's config
        ret = ioctl(irqChipCache.fd, GPIO_V2_GET_LINE_IOCTL, &request);
        if (ret < 0 || request.fd <= 0) {
            std::string msg = "[attachInterrupt] Could not get line handle from ioctl; ";
            msg += strerror(errno);
            throw IRQException(msg);
            return 0;
        }
        irqChipCache.closeDevice();

        ret = ioctl(request.fd, GPIO_V2_LINE_SET_CONFIG_IOCTL, &request.config);
        if (ret < 0) {
            std::string msg = "[attachInterrupt] Could not set line config; ";
            msg += strerror(errno);
            throw IRQException(msg);
            return 0;
        }

        // cache details
        IrqPinCache irqPinCache;
        irqPinCache.fd = request.fd;
        irqPinCache.function = function;

        std::pair<std::map<pinnacle_gpio_t, IrqPinCache>::iterator, bool> indexPair = irqCache.insert(std::pair<pinnacle_gpio_t, IrqPinCache>(pin, irqPinCache));
        if (!indexPair.second) {
            // this should not be reached, but indexPair.first needs to be the inserted map element
            throw IRQException("[attachInterrupt] Could not cache the IRQ pin with function pointer");
            return 0;
        }

        std::pair<std::map<pinnacle_gpio_t, gpio_fd>::iterator, bool> gpioPair = IrqChipCache::cachedPins.insert(std::pair<pinnacle_gpio_t, gpio_fd>(pin, request.fd));
        if (!gpioPair.second) {
            // this should not be reached, but indexPair.first needs to be the inserted map element
            throw IRQException("[attachInterrupt] Could not cache the GPIO pin's file descriptor");
            return 0;
        }

        // create and start thread
        pthread_mutex_lock(&irq_mutex);
        pthread_create(&indexPair.first->second.id, nullptr, poll_irq, &indexPair.first->second);
        pthread_mutex_unlock(&irq_mutex);

        return 1;
    }

    bool detachInterrupt(pinnacle_gpio_t pin)
    {
        std::map<pinnacle_gpio_t, IrqPinCache>::iterator cachedPin = irqCache.find(pin);
        if (cachedPin == irqCache.end()) {
            return 0; // pin not in cache; just exit
        }
        pthread_cancel(cachedPin->second.id);     // send cancel request
        pthread_join(cachedPin->second.id, NULL); // wait till thread terminates
        close(cachedPin->second.fd);
        irqCache.erase(cachedPin);
        GPIOClass::close(pin);
        return 1;
    }

} // namespace cirque_pinnacle_arduino_wrappers

    #ifdef __cplusplus
}
    #endif

#endif // !defined(ARDUINO)
