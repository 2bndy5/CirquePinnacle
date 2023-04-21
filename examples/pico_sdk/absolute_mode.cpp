/*
 * This example reads data from the Cirque trackpad in "absolute mode" and prints the values.
 *
 * See documentation at https://cirquepinnacle.rtfd.io/
 */
#include <cmath>            // sqrt(), pow(), atan2()
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
AbsoluteReport data;

#ifndef PI
    #define PI 3.14159
#endif

/*
Showing all the printed output below will slow down the board's ability to
read() data from the trackpad in a timely manner (resulting in data loss).
Change this variable (via Serial input) to switch between printing
raw data (false) or trigonometric data (true)
*/
bool onlyShowTrigVals = false;

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

#ifndef USE_SW_DR
    printf("-- Using HW Data Ready pin\n");
#endif
#ifndef USE_I2C
    printf("-- Using SPI interface\n");
#else
    printf("-- Using I2C interface\n");
#endif
    printf("\n*** Enter 'M' to measure and print raw data.");
    printf("\n*** Enter 'T' to measure and print trigonometric calculations.");
    printf("\n*** Enter 'B' to reset to bootloader.\n");
    printf("\nTouch the trackpad to see the data\n");
    return true;
}

void loop()
{
    if (trackpad.available()) {
        trackpad.read(&data);

        // datasheet recommends clamping the axes value to reliable range
        if (data.z) { // only clamp values if Z axis is not idle.

            data.x = data.x > 1920 ? 1920 : (data.x < 128 ? 128 : data.x); // 128 <= x <= 1920
            data.y = data.y > 1472 ? 1472 : (data.y < 64 ? 64 : data.y);   //  64 <= y <= 1472
        }

        if (!onlyShowTrigVals) {
            // print raw data from the trackpad
            printf("B1:%d B2:%d B3:%d", data.buttons & 1, data.buttons & 2, data.buttons & 4);
            printf("\tX:%d\tY:%d\tZ:%d\n", data.x, data.y, data.z);
        }
        else {
            // print trigonometric calculations from the trackpad data
            if (!data.z) { // only compute angle and radius if touching (or near) the sensor
                printf("Idling\n");
            }
            else {
                // coordinates assume axes have been clamped to recommended ranges
                double coord_x = (int16_t)(data.x) - 960;
                double coord_y = (int16_t)(data.y) - 736; // NOTE: y-axis is inverted by default

                double radius = sqrt(pow(coord_x, 2) + pow(coord_y, 2));
                double angle = atan2(coord_y, coord_x) * 180 / PI; // angle (in degrees) ranges (-180, 180]

                printf("angle: %.2f\tradius: %.2f\n", angle, radius);
            }
        }
    } // end if trackpad.available()

    char input = getchar_timeout_us(0); // get char from buffer for user input
    if (input != PICO_ERROR_TIMEOUT && (input == 'b' || input == 'B')) {
        reset_usb_boot(0, 0); // reset to bootloader
    }
    else if (input != PICO_ERROR_TIMEOUT && (input == 'm' || input == 'M')) {
        onlyShowTrigVals = false;
    }
    else if (input != PICO_ERROR_TIMEOUT && (input == 't' || input == 'T')) {
        onlyShowTrigVals = true;
    }
} //end loop()

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
