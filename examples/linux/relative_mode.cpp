/*
 * This example reads data from the Cirque trackpad in "relative mode" and prints the values.
 *
 * See documentation at https://cirquepinnacle.rtfd.io/
 */
#include <iostream>                        // cout, endl
#include <CirquePinnacle/CirquePinnacle.h> // trackpad object

#ifdef USE_SW_DR // if using PINNACLE_SW_DR
    #define DR_PIN PINNACLE_SW_DR
#endif
#if defined(PINNACLE_DRIVER_mraa) && !defined(DR_PIN)
    #define DR_PIN 22 // GPIO25
#elif !defined(DR_PIN)
    #define DR_PIN 25
#endif
#define SS_PIN 0

#ifndef USE_I2C
PinnacleTouchSPI trackpad = PinnacleTouchSPI(DR_PIN, SS_PIN);
#else // If using I2C, then use the following line (not the line above)
PinnacleTouchI2C trackpad = PinnacleTouchI2C(DR_PIN);
#endif

RelativeReport report;

bool setup()
{
    if (!trackpad.begin()) {
        std::cout << "Cirque Pinnacle not responding!" << std::endl;
        return false;
    }
    std::cout << "CirquePinnacle/examples/linux/relative_mode" << std::endl;
    return true;
}

void loop()
{
    if (trackpad.available()) {
        trackpad.read(&report);
        std::cout << "Left: " << (unsigned int)(report.buttons & 1)
                  << " Right: " << (unsigned int)(report.buttons & 2)
                  << " Middle: " << (unsigned int)(report.buttons & 4)
                  << "\tdelta X: " << (int)(report.x)
                  << "\tdelta Y: " << (int)(report.y)
                  << "\tdelta Scroll: " << (int)(report.scroll) << std::endl;
    }
}

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
