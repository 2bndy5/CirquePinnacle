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
    #include <unistd.h>    // close()
    #include <fcntl.h>     // open()
    #include <sys/ioctl.h> // ioctl()
    #include <errno.h>     // errno
    #include <string.h>    // strerror(), memset(), strcpy()
    #include <map>
    #include "linux/gpio.h"
    #include "gpio.h"

namespace cirque_pinnacle_arduino_wrappers {

// instantiate some global structs to setup cache
// doing this globally ensures the data struct is zero-ed out
typedef int gpio_fd; // for readability
std::map<pinnacle_gpio_t, gpio_fd> cachedPins;
struct gpio_v2_line_request request;
struct gpio_v2_line_values data;

void GPIOChipCache::openDevice()
{
    if (fd < 0) {
        fd = open(chip, O_RDONLY);
        if (fd < 0) {
            std::string msg = "Can't open device ";
            msg += chip;
            msg += "; ";
            msg += strerror(errno);
            throw GPIOException(msg);
            return;
        }
    }
    chipInitialized = true;
}

void GPIOChipCache::closeDevice()
{
    if (fd >= 0) {
        close(fd);
        fd = -1;
    }
}

GPIOChipCache::GPIOChipCache()
{
    request.num_lines = 1;
    strcpy(request.consumer, "CirquePinnacle lib");
    data.mask = 1ULL; // only change value for specified pin
}

GPIOChipCache::~GPIOChipCache()
{
    closeDevice();
    for (std::map<pinnacle_gpio_t, gpio_fd>::iterator i = cachedPins.begin(); i != cachedPins.end(); ++i) {
        if (i->second > 0) {
            close(i->second);
        }
    }
}

// GPIO chip cache manager
GPIOChipCache gpioCache;

GPIOClass::GPIOClass()
{
}

GPIOClass::~GPIOClass()
{
}

void GPIOClass::open(pinnacle_gpio_t port, bool direction)
{
    try {
        gpioCache.openDevice();
    }
    catch (GPIOException& exc) {
        if (gpioCache.chipInitialized) {
            throw exc;
            return;
        }
        gpioCache.chip = "/dev/gpiochip0";
        gpioCache.openDevice();
    }

    // get chip info
    gpiochip_info info;
    memset(&info, 0, sizeof(info));
    int ret = ioctl(gpioCache.fd, GPIO_GET_CHIPINFO_IOCTL, &info);
    if (ret < 0) {
        std::string msg = "Could not gather info about ";
        msg += gpioCache.chip;
        throw GPIOException(msg);
        return;
    }

    if (port > info.lines) {
        std::string msg = "pin number " + std::to_string(port) + " not available for " + gpioCache.chip;
        throw GPIOException(msg);
        return;
    }

    // check if pin is already in use
    std::map<pinnacle_gpio_t, gpio_fd>::iterator pin = cachedPins.find(port);
    if (pin == cachedPins.end()) { // pin not in use; add it to cached request
        request.offsets[0] = port;
        request.fd = 0;
    }
    else {
        request.fd = pin->second;
    }

    if (request.fd <= 0) {
        ret = ioctl(gpioCache.fd, GPIO_V2_GET_LINE_IOCTL, &request);
        if (ret == -1 || request.fd <= 0) {
            std::string msg = "[GPIO::open] Can't get line handle from IOCTL; ";
            msg += strerror(errno);
            throw GPIOException(msg);
            return;
        }
    }
    gpioCache.closeDevice(); // in case other apps want to access it

    // set the pin and direction
    request.config.flags = direction ? GPIO_V2_LINE_FLAG_OUTPUT : GPIO_V2_LINE_FLAG_INPUT;

    ret = ioctl(request.fd, GPIO_V2_LINE_SET_CONFIG_IOCTL, &request.config);
    if (ret == -1) {
        std::string msg = "[gpio::open] Can't set line config; ";
        msg += strerror(errno);
        throw GPIOException(msg);
        return;
    }
    cachedPins.insert(std::pair<pinnacle_gpio_t, gpio_fd>(port, request.fd));
}

void GPIOClass::close(pinnacle_gpio_t port)
{
    std::map<pinnacle_gpio_t, gpio_fd>::iterator pin = cachedPins.find(port);
    if (pin == cachedPins.end()) {
        return;
    }
    if (pin->second > 0) {
        ::close(pin->second);
    }
    cachedPins.erase(pin);
}

bool GPIOClass::read(pinnacle_gpio_t port)
{
    std::map<pinnacle_gpio_t, gpio_fd>::iterator pin = cachedPins.find(port);
    if (pin == cachedPins.end() || pin->second <= 0) {
        throw GPIOException("[GPIO::read] pin not initialized! Use GPIO::open() first");
        return -1;
    }

    data.bits = 0ULL;

    int ret = ioctl(pin->second, GPIO_V2_LINE_GET_VALUES_IOCTL, &data);
    if (ret == -1) {
        std::string msg = "[GPIO::read] Can't get line value from IOCTL; ";
        msg += strerror(errno);
        throw GPIOException(msg);
        return ret;
    }
    return data.bits & 1ULL;
}

void GPIOClass::write(pinnacle_gpio_t port, bool value)
{
    std::map<pinnacle_gpio_t, gpio_fd>::iterator pin = cachedPins.find(port);
    if (pin == cachedPins.end() || pin->second <= 0) {
        throw GPIOException("[GPIO::write] pin not initialized! Use GPIO::open() first");
        return;
    }

    data.bits = (unsigned long long)value;

    int ret = ioctl(pin->second, GPIO_V2_LINE_SET_VALUES_IOCTL, &data);
    if (ret == -1) {
        std::string msg = "[GPIO::write] Can't set line value from IOCTL; ";
        msg += strerror(errno);
        throw GPIOException(msg);
        return;
    }
}

} // namespace cirque_pinnacle_arduino_wrappers

#endif // !defined(ARDUINO)
