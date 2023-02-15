#ifndef CIRQUEPINNACLE_UTILITY_PIGPIO_TIME_KEEPING_H_
#define CIRQUEPINNACLE_UTILITY_PIGPIO_TIME_KEEPING_H_
#ifndef ARDUINO

namespace cirque_pinnacle_arduino_wrappers {

    #ifdef __cplusplus
extern "C" {
    #endif

    #include <cstdint>

void __msleep(int milisec);

void __usleep(int milisec);

void __start_timer();

uint32_t __millis();

    #define delay(milisec)          __msleep(milisec)
    #define delayMicroseconds(usec) __usleep(usec)
    #define millis()                __millis()

    #ifdef __cplusplus
}
    #endif

} // namespace cirque_pinnacle_arduino_wrappers

#endif // !defined(ARDUINO)
#endif // CIRQUEPINNACLE_UTILITY_PIGPIO_TIME_KEEPING_H_
