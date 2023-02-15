#ifndef CIRQUEPINNACLE_UTILITY_RP2_TIME_KEEPING_H_
#define CIRQUEPINNACLE_UTILITY_RP2_TIME_KEEPING_H_
#ifndef ARDUINO

    #include "pico/stdlib.h"

namespace cirque_pinnacle_arduino_wrappers {

    #define delay(milisec)          sleep_ms(milisec)
    #define delayMicroseconds(usec) sleep_us(usec)
    #define millis()                to_ms_since_boot(get_absolute_time())

} // namespace cirque_pinnacle_arduino_wrappers

#endif // !defined(ARDUINO)
#endif // CIRQUEPINNACLE_UTILITY_RP2_TIME_KEEPING_H_
