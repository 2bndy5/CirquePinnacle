/*
 * This example reads data from the Cirque trackpad in "relative mode" and prints the values.
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

// an object to hold data reported by the Cirque trackpad
RelativeReport data;

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
    printf("CirquePinnacle/examples/pico_sdk/relative_mode\n");

#ifndef USE_SW_DR
    printf("-- Using HW Data Ready pin\n");
#endif
#ifndef USE_I2C
    printf("-- Using SPI interface\n");
#else
    printf("-- Using I2C interface\n");
#endif
    printf("\n*** Enter 'B' to reset to bootloader.\n");
    printf("\nTouch the trackpad to see the data\n");
    return true;
}

void loop()
{
    if (trackpad.available()) {
        trackpad.read(&data);
        printf("Left:%d ", data.buttons & 1);
        printf("Right:%d ", data.buttons & 2);
        printf("Middle:%d", data.buttons & 4);
        printf("\tX: %d\tY: %d\tScroll: %d\n", data.x, data.y, data.scroll);
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
