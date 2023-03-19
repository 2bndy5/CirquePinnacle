/*
 * This example reads data from the Cirque trackpad in "absolute mode" and prints the values.
 *
 * See documentation at https://cirquepinnacle.rtfd.io/
 */
#include "pico/stdlib.h"    // printf(), getchar_timeout_us()
#include "pico/bootrom.h"   // reset_usb_boot()
#include <tusb.h>           // tud_cdc_connected()
#include <CirquePinnacle.h> // trackpad  object
#include "defaultPins.h"    // board presumptive default pin numbers for SS_PIN and DR_PIN

PinnacleTouchSPI trackpad = PinnacleTouchSPI(DR_PIN, SS_PIN);
// If using I2C, then use the following line (not the line above)
// PinnacleTouchI2C trackpad = PinnacleTouchI2C(DR_PIN);

AbsoluteReport report;

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
    printf("CirquePinnacle/examples/pico_sdk/absolute_mode\n");
    trackpad.setDataMode(PINNACLE_ABSOLUTE);
    trackpad.absoluteModeConfig(1); // set count of z-idle packets to 1
    return true;
}

void loop()
{
    if (trackpad.available()) {
        trackpad.read(&report);
        printf("Left: %d ", report.buttons & 1);
        printf("Right: %d ", report.buttons & 2);
        printf("Middle: %d", report.buttons & 4);
        printf("\tX: %d\tY: %d\tZ: %d\n", report.x, report.y, report.z);
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
