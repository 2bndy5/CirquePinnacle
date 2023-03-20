/*
 * This example reads data from the Cirque trackpad in "absolute mode" and prints the values.
 *
 * See documentation at https://cirquepinnacle.rtfd.io/
 */
#include <iostream>                        // cout, endl
#include <CirquePinnacle/CirquePinnacle.h> // trackpad object

#ifdef PINNACLE_DRIVER_mraa
    #include <mraa/types.hpp>
    #define DR_PIN mraa::RaspberryWiring::RASPBERRY_WIRING_PIN22 // GPIO25
#else
    #define DR_PIN 25
#endif
#define SS_PIN 0

PinnacleTouchSPI trackpad = PinnacleTouchSPI(DR_PIN, SS_PIN);
// If using I2C, then use the following line (not the line above)
// PinnacleTouchI2C trackpad = PinnacleTouchI2C(DR_PIN);

AbsoluteReport report;

bool setup()
{
    if (!trackpad.begin()) {
        std::cout << "Cirque Pinnacle not responding!" << std::endl;
        return false;
    }
    std::cout << "CirquePinnacle/examples/linux/absolute_mode" << std::endl;
    trackpad.setDataMode(PINNACLE_ABSOLUTE);
    trackpad.absoluteModeConfig(1); // set count of z-idle packets to 1
    return true;
}

void loop()
{
    if (trackpad.available()) {
        trackpad.read(&report);
        std::cout << "button1: " << (unsigned int)(report.buttons & 1)
                  << " button2: " << (unsigned int)(report.buttons & 2)
                  << " button3: " << (unsigned int)(report.buttons & 4)
                  << "\tX: " << (unsigned int)(report.x)
                  << "\tY: " << (unsigned int)(report.y)
                  << "\tZ: " << (unsigned int)(report.z) << std::endl;
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
