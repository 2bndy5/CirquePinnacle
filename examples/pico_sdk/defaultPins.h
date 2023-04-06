// pre-chosen pins for different boards
#ifndef DEFAULTPINS_H
#define DEFAULTPINS_H

#ifdef USE_SW_DR // if using PINNACLE_SW_DR
    #define DR_PIN PINNACLE_SW_DR
#endif

#if defined(ADAFRUIT_QTPY_RP2040)
    // for this board, you can still use the Stemma QT connector as a separate I2C bus (`i2c1`)
    #define SS_PIN PICO_DEFAULT_UART_RX_PIN // the pin labeled RX (GPIO5)
    #ifndef DR_PIN // if not using PINNACLE_SW_DR
        #define DR_PIN PICO_DEFAULT_UART_TX_PIN // the pin labeled TX (GPIO20)
    #endif

#elif defined(PIMORONI_TINY2040)
    #define SS_PIN PICO_DEFAULT_SPI_CSN_PIN // pin 5
    #ifndef DR_PIN // if not using PINNACLE_SW_DR
        #define DR_PIN PICO_DEFAULT_I2C_SDA_PIN // pin 2
    #endif

#elif defined(SPARFUN_THINGPLUS)
    #define SS_PIN 16 // the pin labeled 16
    #ifndef DR_PIN // if not using PINNACLE_SW_DR
        #define DR_PIN 6 // the pin labeled SDA
    #endif

#elif defined(ADAFRUIT_ITSYBITSY_RP2040)
    #define SS_PIN 12 // the pin labeled 2
    #ifndef DR_PIN // if not using PINNACLE_SW_DR
        #define DR_PIN 6 // the pin labeled 7
    #endif

#else
    // pins available on (ADAFRUIT_FEATHER_RP2040 || Pico_board || Sparkfun_ProMicro || SparkFun MicroMod)

    #define SS_PIN 6
    #ifndef DR_PIN // if not using PINNACLE_SW_DR
        #define DR_PIN 8
    #endif
#endif // board detection macro defs

#endif // DEFAULTPINS_H
