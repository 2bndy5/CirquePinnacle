#ifndef CIRQUEPINNACLE_UTILITY_BCM2XXX_GPIO_H_
#define CIRQUEPINNACLE_UTILITY_BCM2XXX_GPIO_H_
#ifndef ARDUINO

    #include "bcm2835.h"

typedef uint8_t pinnacle_gpio_t;

namespace cirque_pinnacle_arduino_wrappers {

// include this header in other /utility/bcm2xxx headers to init the lib globally.
static bool bcmInitialized = (bool)bcm2835_init();

    #define INPUT  BCM2835_GPIO_FSEL_INPT
    #define OUTPUT BCM2835_GPIO_FSEL_OUTP
    // HIGH and LOW are declared in bcm2835.h
    #define digitalRead(pin)         bcm2835_gpio_lev(pin)
    #define digitalWrite(pin, value) bcm2835_gpio_write(pin, value)
    #define pinMode(pin, value)      bcm2835_gpio_fsel(pin, value)

} // namespace cirque_pinnacle_arduino_wrappers

#endif // !defined(ARDUINO)
#endif // CIRQUEPINNACLE_UTILITY_BCM2XXX_GPIO_H_
