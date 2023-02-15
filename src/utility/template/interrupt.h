#ifndef CIRQUEPINNACLE_UTILITY_PIGPIO_INTERRUPT_H_
#define CIRQUEPINNACLE_UTILITY_PIGPIO_INTERRUPT_H_
#ifndef ARDUINO

namespace cirque_pinnacle_arduino_wrappers {

    #define INT_EDGE_SETUP   0
    #define INT_EDGE_FALLING FALLING_EDGE
    #define INT_EDGE_RISING  RISING_EDGE
    #define INT_EDGE_BOTH    EITHER_EDGE

    #ifdef __cplusplus
extern "C" {
    #endif

extern int attachInterrupt(int pin, int mode, void (*function)(void));

extern int detachInterrupt(int pin);

    #ifdef __cplusplus
}
    #endif

} // namespace cirque_pinnacle_arduino_wrappers

#endif // !defined(ARDUINO)
#endif // CIRQUEPINNACLE_UTILITY_PIGPIO_INTERRUPT_H_