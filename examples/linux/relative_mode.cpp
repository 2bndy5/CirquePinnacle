/*
 * This example reads data from the Cirque trackpad in "relative mode" and prints the values.
 *
 * See documentation at https://cirquepinnacle.rtfd.io/
 */
#include <iostream>                        // cout, endl
#include <CirquePinnacle/CirquePinnacle.h> // trackpad object

#ifdef PINNACLE_DRIVER_mraa
    #define DR_PIN 22 // GPIO25
#else
    #define DR_PIN 25
#endif
#define SS_PIN 0

PinnacleTouchSPI trackpad = PinnacleTouchSPI(DR_PIN, SS_PIN);
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

        uint8_t* buf = (uint8_t*)(&report);
        for (uint8_t i = 0; i < sizeof(report); ++i) {
            std::cout << std::hex << (unsigned int)(buf[i]) << " ";
        }
        std::cout << std::dec << std::endl;
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
