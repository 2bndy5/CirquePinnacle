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
