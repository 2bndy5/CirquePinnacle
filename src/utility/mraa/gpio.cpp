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
    std::map<pinnacle_gpio_t, mraa::Gpio*>::iterator i = cache.find(port);
    if (i == cache.end()) {
        mraa::Gpio* gpio_inst = new mraa::Gpio(port);
        cache[port] = gpio_inst;
        gpio_inst->dir(direction);
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
    throw GPIOException("pin was not initialized with GPIO::open()");
    return -1;
}

void GPIOClass::write(pinnacle_gpio_t port, int value)
{
    mraa::Result result = mraa::Result::ERROR_UNSPECIFIED; // a default
    // get cache gpio instance
    std::map<pinnacle_gpio_t, mraa::Gpio*>::iterator i = cache.find(port);
    if (i != cache.end()) {
        result = i->second->write(value);
    }
    else {
        throw GPIOException("pin was not initialized with GPIO::open()");
    }

    if (result != mraa::Result::SUCCESS) {
        throw GPIOException("mraa::Gpio.write() failed");
    }
}

} // namespace cirque_pinnacle_arduino_wrappers

#endif // !defined(ARDUINO)
