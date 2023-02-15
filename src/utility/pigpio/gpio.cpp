#ifndef ARDUINO
    #include <pigpio.h>
    #include "gpio.h"

namespace cirque_pinnacle_arduino_wrappers {

bool initialized = 0;

GPIOClass::GPIOClass()
{
}

GPIOClass::~GPIOClass()
{
    gpioTerminate();
}

void GPIOClass::open(pinnacle_gpio_t port, int direction)
{
    if (!initialized) {
        gpioInitialise();
    }
    initialized = true;
    gpioSetMode(port, direction);
}

void GPIOClass::close(pinnacle_gpio_t port)
{
    // see destructor as it clears initialized pins.
}

int GPIOClass::read(pinnacle_gpio_t port)
{
    return gpioRead(port);
}

void GPIOClass::write(pinnacle_gpio_t port, int value)
{
    gpioWrite(port, value);
}

} // namespace cirque_pinnacle_arduino_wrappers

#endif // !defined(ARDUINO)
