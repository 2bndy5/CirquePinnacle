#include "pico/stdlib.h"    // printf(), sleep_ms(), getchar_timeout_us(), to_us_since_boot(), get_absolute_time()
#include "pico/bootrom.h"   // reset_usb_boot()
#include <tusb.h>           // tud_cdc_connected()
#include <CirquePinnacle.h> // trackpad  object
#include "defaultPins.h"    // board presumptive default pin numbers for CE_PIN and CSN_PIN

PinnacleTouchSPI trackpad = PinnacleTouchSPI(SS_PIN, DR_PIN);
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
    printf("found Cirque Pinnacle!\n");
    trackpad.setDataMode(PINNACLE_ABSOLUTE);
    trackpad.absoluteModeConfig(1); // set count of z-idle packets to 1
    return true;
}

void loop()
{
    if (trackpad.available()) {
        trackpad.read(&report);
        printf("Left: ");
        printf("%d", report.buttons & 1);
        printf(" Right: ");
        printf("%d", report.buttons & 2);
        printf(" Middle: ");
        printf("%d", report.buttons & 4);
        printf("\tX: ");
        printf("%d", report.x);
        printf("\tY: ");
        printf("%d", report.y);
        printf("\tZ: ");
        printf("%d\n", report.z);
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