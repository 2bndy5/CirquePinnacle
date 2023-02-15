#ifndef CIRQUEPINNACLE_UTILITY_PIGPIO_INTERRUPT_H_
#define CIRQUEPINNACLE_UTILITY_PIGPIO_INTERRUPT_H_
#ifndef ARDUINO

    #include <pigpio.h>

namespace arduino_wrappers {

    #define INT_EDGE_SETUP   0
    #define INT_EDGE_FALLING FALLING_EDGE
    #define INT_EDGE_RISING  RISING_EDGE
    #define INT_EDGE_BOTH    EITHER_EDGE

    #ifdef __cplusplus
extern "C" {
    #endif

/*
 * attachInterrupt (Original: wiringPiISR):
 *      Pi Specific.
 *      Take the details and create an interrupt handler that will do a call-
 *      back to the user supplied function.
 *********************************************************************************
 */
extern int attachInterrupt(int pin, int mode, void (*function)(void));

/*
 * detachInterrupt:
 *      Pi Specific detachInterrupt.
 *      Will cancel the interrupt thread, close the filehandle and 
 *		setting wiringPi back to 'none' mode.
 *********************************************************************************
 */
extern int detachInterrupt(int pin);

    #ifdef __cplusplus
}
    #endif

} // namespace arduino_wrappers

#endif // !defined(ARDUINO)
#endif // CIRQUEPINNACLE_UTILITY_PIGPIO_INTERRUPT_H_
