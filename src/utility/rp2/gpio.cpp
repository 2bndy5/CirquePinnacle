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

GPIOClass::GPIOClass()
{
}

void GPIOClass::open(pinnacle_gpio_t port, bool direction)
{
    gpio_init(port);
    gpio_set_dir(port, direction);
}

void GPIOClass::close(pinnacle_gpio_t port)
{
    gpio_init(port);
}

bool GPIOClass::read(pinnacle_gpio_t port)
{
    return gpio_get(port);
}

void GPIOClass::write(pinnacle_gpio_t port, bool value)
{
    gpio_put(port, value);
}

GPIOClass::~GPIOClass()
{
}

} // namespace cirque_pinnacle_arduino_wrappers

#endif // !defined(ARDUINO)
