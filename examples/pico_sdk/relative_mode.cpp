/*
 * This example reads data from the Cirque trackpad in "relative mode" and prints the values.
 *
 * See documentation at https://cirquepinnacle.rtfd.io/
 */
#include "pico/stdlib.h"    // printf(), sleep_ms(), getchar_timeout_us(), to_us_since_boot(), get_absolute_time()
#include "pico/bootrom.h"   // reset_usb_boot()
#include <tusb.h>           // tud_cdc_connected()
#include <CirquePinnacle.h> // trackpad  object
#include "defaultPins.h"    // board presumptive default pin numbers for CE_PIN and CSN_PIN

PinnacleTouchSPI trackpad = PinnacleTouchSPI(DR_PIN, SS_PIN);
RelativeReport report;

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
    // else
    printf("found Cirque Pinnacle!\n");
    return true;
}

void loop()
{
    if (trackpad.available()) {
        trackpad.read(&report);
        printf("Left: %d ", report.buttons & 1);
        printf("Right: %d ", report.buttons & 2);
        printf("Middle: %d", report.buttons & 4);
        printf("\tdelta X: %d", report.x);
        printf("\tdelta Y: %d", report.y);
        printf("\tdelta Scroll: %d\n", report.scroll);
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
