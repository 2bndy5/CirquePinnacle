
#ifndef CIRQUEPINNACLE_UTILITY_TEMPLATE_TIME_KEEPING_H_
#define CIRQUEPINNACLE_UTILITY_TEMPLATE_TIME_KEEPING_H_
#ifndef ARDUINO

    #include <cstdint>

namespace cirque_pinnacle_arduino_wrappers {

    #ifdef __cplusplus
extern "C" {
    #endif

void __msleep(int milliseconds);

void __usleep(int microseconds);

uint32_t __millis();

    #define delay(ms)             __msleep(ms)
    #define delayMicroseconds(us) __usleep(us)
    #define millis()              __millis()

    #ifdef __cplusplus
}
    #endif

} // namespace cirque_pinnacle_arduino_wrappers

#endif // !defined(ARDUINO)
#endif // CIRQUEPINNACLE_UTILITY_TEMPLATE_TIME_KEEPING_H_