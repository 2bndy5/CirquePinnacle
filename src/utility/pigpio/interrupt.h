/**
 * Interrupt functions
 */
#ifndef CIRQUEPINNACLE_UTILITY_PIGPIO_INTERRUPT_H_
#define CIRQUEPINNACLE_UTILITY_PIGPIO_INTERRUPT_H_
#ifndef ARDUINO

    #include <pigpio.h> // FALLING_EDGE, RISING_EDGE, EITHER_EDGE
    #include "gpio.h"   // pinnacle_gpio_t

    #ifdef __cplusplus
extern "C" {
    #endif

namespace cirque_pinnacle_arduino_wrappers {

    enum Edge : uint8_t
    {
        FALLING = FALLING_EDGE,
        RISING = RISING_EDGE,
        CHANGE = EITHER_EDGE,
    };

    /**
     * Take the details and create an interrupt handler that will
     * callback to the user-supplied function.
     */
    int attachInterrupt(pinnacle_gpio_t pin, uint8_t mode, void (*function)(void));

    /**
     * Will cancel the interrupt thread, close the filehandle and release the pin.
     */
    int detachInterrupt(pinnacle_gpio_t pin);

} // namespace cirque_pinnacle_arduino_wrappers

    #ifdef __cplusplus
}
    #endif

#endif // !defined(ARDUINO)
#endif // CIRQUEPINNACLE_UTILITY_PIGPIO_INTERRUPT_H_
