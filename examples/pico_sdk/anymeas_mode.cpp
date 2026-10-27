/*
 * This example reads data from the Cirque trackpad in "anymeas mode" and prints the values.
 *
 * See documentation at https://cirquepinnacle.rtfd.io/
 */
#include "pico/stdlib.h"    // printf(), getchar_timeout_us()
#include "pico/bootrom.h"   // reset_usb_boot()
#include <tusb.h>           // tud_cdc_connected()
#include <CirquePinnacle.h> // trackpad  object
#include "defaultPins.h"    // board presumptive default pin numbers for SS_PIN and DR_PIN

#ifndef USE_I2C
PinnacleTouchSPI trackpad(DR_PIN, SS_PIN);
#else // If using I2C, then use the following line (not the line above)
PinnacleTouchI2C trackpad(DR_PIN);
#endif

// the index number used to iterate through our vectorDeterminants array used in loop()
unsigned int vectorIndex = 0;

// interrupt related handling
volatile bool isDataReady = false; // track the interrupts with our own IRQ flag
bool waitingForInterrupt = false;  // a flag to control iteration of our loop()
/// A callback function that allows `loop()` to know when the trackpad's DR pin is active
void interruptHandler(uint gpio, uint32_t events)
{
    if (gpio != DR_PIN && !(events & GPIO_IRQ_EDGE_RISE)) {
        // the gpio pin and event does not match the configuration we specified
        return;
    }
    isDataReady = true; // forward event handling back to main loop()
}

typedef struct _MeasureVector
{
    unsigned long toggle;
    unsigned long polarity;
} measureVector;

measureVector vectorDeterminants[] = {
    // toggle  ,   polarity
    {0x0000FFFF, 0x00000000}, // toggle all x-axis bits negatively (to 0)
    {0x0FFF0000, 0x00000000}, // toggle all y-axis bits negatively (to 0)
    {0x00FF00FF, 0x000000FF}, // toggle Y0-Y7 negatively (to 0) & X0-X7 positively (to 1)
    {0x00FF00FF, 0x00FF0000}  // toggle Y0-Y7 positively (to 1) & X0-X7 negatively (to 0)
};

const uint8_t variousVectors_size = sizeof(vectorDeterminants) / sizeof(measureVector);
int16_t compensations[variousVectors_size];

void compensate()
{
    int32_t accumulatedValue;
    for (uint8_t i = 0; i < variousVectors_size; ++i) {
        uint8_t sweep = 0;
        accumulatedValue = 0;
        while (sweep < 5) // take 5 measurements and average them for a bit lower noise compensation value
        {
            int16_t value = trackpad.measureAdc(vectorDeterminants[i].toggle, vectorDeterminants[i].polarity);
            sweep++;
            accumulatedValue += value;
        }
        compensations[i] = accumulatedValue / 5;
        printf("compensation %d: %d\n", i, compensations[i]);
    }
}

bool setup()
{
    // wait here until the CDC ACM (serial port emulation) is connected
    while (!tud_cdc_connected()) {
        sleep_ms(10);
    }

    if (!trackpad.begin()) {
        printf("Cirque Pinnacle not responding!\n");
        return false;
    }
    printf("CirquePinnacle/examples/pico_sdk/anymeas_mode\n");
    trackpad.setDataMode(PINNACLE_ANYMEAS);

#ifndef USE_I2C
    printf("-- Using SPI interface\n");
#else
    printf("-- Using I2C interface\n");
#endif
    printf("\n*** Enter 'B' to reset to bootloader.\n");
    compensate();

    // setup interrupt handler.
    // We do this AFTER calling `compensate()` because
    // `compensate()` will unnecessarily trigger `interruptHandler()`
    gpio_set_irq_enabled_with_callback(DR_PIN, GPIO_IRQ_EDGE_RISE, true, &interruptHandler);

    for (uint8_t i = 5; i; --i) {
        printf("starting in %d second%c\r", i, i > 1 ? 's' : ' ');
        sleep_ms(1000);
    }
    return true;
}

void loop()
{
    if (!isDataReady && !waitingForInterrupt) {
        trackpad.startMeasureAdc(
            vectorDeterminants[vectorIndex].toggle,
            vectorDeterminants[vectorIndex].polarity);
        waitingForInterrupt = true;
    }
    else if (isDataReady) {
        isDataReady = false;         // reset our IRQ flag
        waitingForInterrupt = false; // allow iteration to continue

        int16_t measurement = trackpad.getMeasureAdc();
        measurement -= compensations[vectorIndex];
        printf("meas%d: %d\t", vectorIndex, measurement);
        // increment our loop iterator
        if (vectorIndex < (variousVectors_size - 1)) {
            vectorIndex++;
        }
        else {
            vectorIndex = 0;
            printf("\n");
        }
    }

    char input = getchar_timeout_us(0); // get char from buffer for user input
    if (input != PICO_ERROR_TIMEOUT && (input == 'b' || input == 'B')) {
        // reset to bootloader
        trackpad.shutdown(true);
        reset_usb_boot(0, 0);
    }
}

int main()
{
    stdio_init_all(); // init necessary IO for the RP2040

    while (!setup()) { // if trackpad.begin() failed
        // hold program in infinite attempts to initialize trackpad
    }
    while (true) {
        loop();
    }
    return 0; // we will never reach this
}
