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

PinnacleTouchSPI trackpad = PinnacleTouchSPI(SS_PIN, DR_PIN);

typedef struct _MeasureVector
{
    unsigned long toggle;
    unsigned long polarity;
} measureVector;

measureVector vectorDeterminants[] = {
    // {toggle, polarity}
    {0x0000FFFF, 0x00000000}, // toggle all x-axis bits negatively (to 0)
    {0x0FFF0000, 0x00000000}, // toggle all y-axis bits negatively (to 0)
    {0x00FF00FF, 0x000000FF}, // toggle Y0-Y7 negatively (to 0) & X0-X7 positively (to 1)
    {0x00FF00FF, 0x00FF0000}  // toggle Y0-Y7 positively (to 1) & X0-X7 negatively (to 0)
};

uint8_t variousVectors_size = sizeof(vectorDeterminants) / sizeof(measureVector);

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
    printf("found Cirque Pinnacle!\n");
    trackpad.setDataMode(PINNACLE_ANYMEAS);
    return true;
}

void loop()
{
    for (uint8_t i = 0; i < variousVectors_size; i++) {
        int16_t measurement = trackpad.measureAdc(
            vectorDeterminants[i].toggle,
            vectorDeterminants[i].polarity);
        printf("meas%d: %d\t", i, measurement);
    }
    printf("\n");

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
