#ifndef ARDUINO
    #include "gpio.h"

namespace cirque_pinnacle_arduino_wrappers {

// cache for mraa::Gpio instances
std::map<int, mraa::Gpio*> GPIOClass::cache;

GPIOClass::GPIOClass()
{
}

GPIOClass::~GPIOClass()
{
    // deinitialize cache of mraa::Gpio instances/pointers
    std::map<int, mraa::Gpio*>::iterator i;
    for (i = cache.begin(); i != cache.end(); i++) {
        i->second->close();
    }
    cache.clear();
}

void GPIOClass::open(pinnacle_gpio_t port, mraa::Dir direction)
{
    // check that mraa::Gpio context doesn't already exist
    std::map<int, mraa::Gpio*>::iterator i = cache.find(port);
    if (i == cache.end()) {
        mraa::Gpio* gpio_inst = new mraa::Gpio(port, 0);
        cache[port] = gpio_inst;
        gpio_inst->dir(direction);
    }
}

void GPIOClass::close(pinnacle_gpio_t port)
{
    // check that mraa::Gpio context exists, meaning GPIO::open() was called.
    std::map<int, mraa::Gpio*>::iterator i = cache.find(port);
    if (i != cache.end()) {
        i->second->close(); // close the cached Gpio instance
        cache.erase(i);     // Delete cache entry
    }
}

int GPIOClass::read(pinnacle_gpio_t port)
{
    // get cache gpio instance
    std::map<int, mraa::Gpio*>::iterator i = cache.find(port);
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
    std::map<int, mraa::Gpio*>::iterator i = cache.find(port);
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
