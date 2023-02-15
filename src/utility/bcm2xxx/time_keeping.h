#ifndef CIRQUEPINNACLE_UTILITY_BCM2XXX_TIME_KEEPING_H_
#define CIRQUEPINNACLE_UTILITY_BCM2XXX_TIME_KEEPING_H_
#ifndef ARDUINO

    #include <cstdint>

namespace cirque_pinnacle_arduino_wrappers {

    #ifdef __cplusplus
extern "C" {
    #endif

// the bcm2835.h defines the following functions for us: `delay()`, `delayMicroseconds()`

uint32_t __millis();

    #define millis() __millis()

    #ifdef __cplusplus
}
    #endif

} // namespace cirque_pinnacle_arduino_wrappers

#endif // !defined(ARDUINO)
#endif // CIRQUEPINNACLE_UTILITY_BCM2XXX_TIME_KEEPING_H_
