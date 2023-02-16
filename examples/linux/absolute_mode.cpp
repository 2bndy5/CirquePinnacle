/*
 * This example reads data from the Cirque trackpad in "absolute mode" and prints the values.
 *
 * See documentation at https://cirquepinnacle.rtfd.io/
 */
#include <iostream>                        // cout, endl
#include <CirquePinnacle/CirquePinnacle.h> // trackpad  object

#define DR_PIN 6
#define SS_PIN 0

PinnacleTouchSPI trackpad = PinnacleTouchSPI(DR_PIN, SS_PIN);
AbsoluteReport report;

bool setup()
{
    if (!trackpad.begin()) {
        std::cout << "Cirque Pinnacle not responding!" << std::endl;
        return false;
    }
    std::cout << "found Cirque Pinnacle!" << std::endl;
    trackpad.setDataMode(PINNACLE_ABSOLUTE);
    trackpad.absoluteModeConfig(1); // set count of z-idle packets to 1
    return true;
}

void loop()
{
    if (trackpad.available()) {
        trackpad.read(&report);
        std::cout << "Left: " << (unsigned int)(report.buttons & 1)
                  << " Right: " << (unsigned int)(report.buttons & 2)
                  << " Middle: " << (unsigned int)(report.buttons & 4)
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
