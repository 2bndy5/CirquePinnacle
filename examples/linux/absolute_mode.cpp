/*
 * This example reads data from the Cirque trackpad in "absolute mode" and prints the values.
 *
 * See documentation at https://cirquepinnacle.rtfd.io/
 */
#include <cmath>                           // sqrt(), pow(), atan2(), M_PI
#include <iostream>                        // cout, endl, cin
#include <iomanip>                         // setprecision()
#include <CirquePinnacle/CirquePinnacle.h> // trackpad object

#define DR_PIN 25 // GPIO25
#define SS_PIN 0

#ifndef USE_I2C
PinnacleTouchSPI trackpad(DR_PIN, SS_PIN);
#else // If using I2C, then use the following line (not the line above)
PinnacleTouchI2C trackpad(DR_PIN);
#endif

// an object to hold data reported by the Cirque trackpad
AbsoluteReport data;

// track the interrupts with our own IRQ flag
volatile bool isDataReady = false;

void interruptHandler()
{
    isDataReady = true;
}

/*
Showing all the printed output below will slow down the board's ability to
read() data from the trackpad in a timely manner (resulting in data loss).
Use this compile-time definition to switch between printing
raw data (false) or trigonometry data (true)
*/
static bool onlyShowTrigVals;

bool setup()
{
    if (!trackpad.begin()) {
        std::cout << "Cirque Pinnacle not responding!" << std::endl;
        return false;
    }
    std::cout << "CirquePinnacle/examples/linux/absolute_mode\n"
              << std::endl;
    trackpad.setDataMode(PINNACLE_ABSOLUTE);
    trackpad.absoluteModeConfig(1); // set count of z-idle packets to 1

    // pull in arduino-like namespace
    namespace arduino = cirque_pinnacle_arduino_wrappers;
    // setup the interrupt handler
    arduino::attachInterrupt(DR_PIN, &interruptHandler, arduino::RISING);

#ifndef USE_I2C
    std::cout << "-- Using SPI interface." << std::endl;
#else
    std::cout << "-- Using I2C interface." << std::endl;
#endif
    std::cout << "\nShow trigonometric calculations? [y/N] ('N' means show raw data) ";
    char input[4] = {0};
    std::cin.getline(input, 3);
    onlyShowTrigVals = input[0] == 'y' || input[0] == 'Y';
    std::cout << "showing ";
    if (onlyShowTrigVals)
        std::cout << "trigonometric calculations." << std::endl;
    else
        std::cout << "raw data." << std::endl;
    std::cout << "\nTouch the trackpad to see the data" << std::endl;
    return true;
}

void loop()
{
    // using `interruptHandler()` to update `isDataReady`
    if (isDataReady) {
        // assert(isDataReady == trackpad.available());
        isDataReady = false; // reset our IRQ flag
        trackpad.read(&data);

        // datasheet recommends clamping the axes value to reliable range
        if (data.z) { // only clamp values if Z axis is not idle.

            data.x = data.x > 1920 ? 1920 : (data.x < 128 ? 128 : data.x); // 128 <= x <= 1920
            data.y = data.y > 1472 ? 1472 : (data.y < 64 ? 64 : data.y);   //  64 <= y <= 1472
        }

        if (!onlyShowTrigVals) {
            // print raw data from the trackpad
            std::cout << "B1: " << (unsigned int)(data.buttons & 1)
                      << " B2: " << (unsigned int)(data.buttons & 2)
                      << " B3: " << (unsigned int)(data.buttons & 4)
                      << "\tX: " << data.x
                      << "\tY: " << data.y
                      << "\tZ: " << (unsigned int)(data.z) << std::endl;
        }
        else {
            // print trigonometric data from the trackpad
            if (!data.z) { // only compute angle and radius if touching (or near) the sensor
                std::cout << "Idling" << std::endl;
            }
            else {
                // coordinates assume axes have been clamped to recommended ranges
                double coord_x = (int16_t)(data.x) - 960;
                double coord_y = (int16_t)(data.y) - 736; // NOTE: y-axis is inverted by default

                double radius = sqrt(pow(coord_x, 2) + pow(coord_y, 2));
                double angle = atan2(coord_y, coord_x) * 180 / M_PI; // angle (in degrees) ranges (-180, 180]

                std::cout << std::setprecision(5) << "angle: " << angle << "\tradius: " << radius << std::endl;
            }
        }
    } // end if trackpad.available()
} //end loop()

int main()
{
    if (!setup()) { // if trackpad.begin() failed
        return -1;  // fail fast
    }
    while (true) { // use ctrl+C to exit
        loop();
    }
    return 0; // we will never reach this
}
