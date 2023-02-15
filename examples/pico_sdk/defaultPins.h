// pre-chosen pins for different boards
#ifndef DEFAULTPINS_H
#define DEFAULTPINS_H

#if defined(ADAFRUIT_QTPY_RP2040)
    // for this board, you can still use the Stemma QT connector as a separate I2C bus (`i2c1`)
    #define SS_PIN PICO_DEFAULT_UART_TX_PIN // the pin labeled TX
    #define DR_PIN PICO_DEFAULT_UART_RX_PIN // the pin labeled RX

#elif defined(PIMORONI_TINY2040)
    #define SS_PIN PICO_DEFAULT_SPI_CSN_PIN // pin 5
    #define DR_PIN PICO_DEFAULT_I2C_SDA_PIN // pin 2

#elif defined(SPARFUN_THINGPLUS)
    #define SS_PIN 16 // the pin labeled 16
    #define DR_PIN 6  // the pin labeled SDA

#else
    // pins available on (ADAFRUIT_ITSYBITSY_RP2040 || ADAFRUIT_FEATHER_RP2040 || Pico_board || Sparkfun_ProMicro || SparkFun MicroMod)

    #define SS_PIN 8
    #define DR_PIN 6
#endif // board detection macro defs

#endif // DEFAULTPINS_H
