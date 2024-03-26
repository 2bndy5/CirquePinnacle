/**
 * Interrupt implementation wrapped from pigpio library
 */
#ifndef ARDUINO
    #include <pigpio.h>
    #include "interrupt.h"

    #ifdef __cplusplus
extern "C" {
    #endif

namespace cirque_pinnacle_arduino_wrappers {

    int attachInterrupt(pinnacle_gpio_t pin, uint8_t mode, void (*function)(void))
    {
        gpioInitialise();
        return gpioSetISRFunc(pin, mode, 0, (gpioISRFunc_t)function);
    }

    int detachInterrupt(pinnacle_gpio_t pin)
    {
        return gpioSetISRFunc(pin, 0, 0, NULL);
    }

} // namespace cirque_pinnacle_arduino_wrappers

    #ifdef __cplusplus
}
    #endif

#endif // !defined(ARDUINO)
