// pre-chosen pins for different boards
#ifndef DEFAULTPINS_H
#define DEFAULTPINS_H

#if defined(ADAFRUIT_QTPY_RP2040)
    #define SS_PIN PICO_DEFAULT_UART_RX_PIN // the pin labeled RX (GPIO5)
    #define DR_PIN PICO_DEFAULT_UART_TX_PIN // the pin labeled TX (GPIO20)

#elif defined(PIMORONI_TINY2040)
    #define SS_PIN PICO_DEFAULT_SPI_CSN_PIN // pin 5
    #define DR_PIN 1                        // pin 1

#elif defined(SPARFUN_THINGPLUS)
    #define SS_PIN 21 // the pin labeled 21
    #define DR_PIN 22 // the pin labeled 22

#elif defined(ADAFRUIT_ITSYBITSY_RP2040)
    #define SS_PIN 12 // the pin labeled 2
    #define DR_PIN 6  // the pin labeled 7

#else
    // pins available on (ADAFRUIT_FEATHER_RP2040 || Pico_board || Sparkfun_ProMicro || SparkFun MicroMod)

    #define SS_PIN 6
    #define DR_PIN 8
#endif // board detection macro defs

#endif // DEFAULTPINS_H
