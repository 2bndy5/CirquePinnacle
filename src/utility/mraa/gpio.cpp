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
    #include <mraa.h>   // mraa_strresult()
    #include <mraa.hpp> // mraa::Gpio
    #include "gpio.h"

namespace cirque_pinnacle_arduino_wrappers {

// cache for mraa::Gpio instances
std::map<pinnacle_gpio_t, mraa::Gpio*> GPIOClass::cache;

GPIOClass::GPIOClass()
{
}

GPIOClass::~GPIOClass()
{
    // deinitialize cache of mraa::Gpio instances/pointers
    std::map<pinnacle_gpio_t, mraa::Gpio*>::iterator i;
    for (i = cache.begin(); i != cache.end(); i++) {
        i->second->close();
    }
    cache.clear();
}

void GPIOClass::open(pinnacle_gpio_t port, mraa::Dir direction)
{
    // check that mraa::Gpio context doesn't already exist
    mraa::Result status;
    std::map<pinnacle_gpio_t, mraa::Gpio*>::iterator i = cache.find(port);
    if (i == cache.end()) {
        mraa::Gpio* gpio_inst = new mraa::Gpio(port);
        cache[port] = gpio_inst;
        status = gpio_inst->dir(direction);
    }
    else {
        status = i->second->dir(direction);
    }
    if (status != mraa::SUCCESS) {
        std::string msg = "[GPIOClass::open] Could not set the pin direction; ";
        msg += mraa_strresult((mraa_result_t)status);
        throw GPIOException(msg);
    }
}

void GPIOClass::close(pinnacle_gpio_t port)
{
    // check that mraa::Gpio context exists, meaning GPIO::open() was called.
    std::map<pinnacle_gpio_t, mraa::Gpio*>::iterator i = cache.find(port);
    if (i != cache.end()) {
        i->second->close(); // close the cached Gpio instance
        cache.erase(i);     // Delete cache entry
    }
}

int GPIOClass::read(pinnacle_gpio_t port)
{
    // get cache gpio instance
    std::map<pinnacle_gpio_t, mraa::Gpio*>::iterator i = cache.find(port);
    if (i != cache.end()) {
        return i->second->read();
    }
    throw GPIOException("[GPIOClass:read] pin was not initialized with GPIO::open()");
    return -1;
}

void GPIOClass::write(pinnacle_gpio_t port, int value)
{
    // get cache gpio instance
    std::map<pinnacle_gpio_t, mraa::Gpio*>::iterator i = cache.find(port);
    if (i != cache.end()) {
        mraa::Result result = i->second->write(value);
        if (result != mraa::Result::SUCCESS) {
            std::string msg = "[GPIOClass::write] Could not set pin output value; ";
            msg += mraa_strresult((mraa_result_t)result);
            throw GPIOException(msg);
        }
    }
    else {
        throw GPIOException("pin was not initialized with GPIO::open()");
    }
}

} // namespace cirque_pinnacle_arduino_wrappers

#endif // !defined(ARDUINO)
