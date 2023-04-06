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
PinnacleTouchSPI trackpad(DR_PIN, SS_PIN);
#else // If using I2C, then use the following line (not the line above)
PinnacleTouchI2C trackpad(DR_PIN);
#endif

// an object to hold data reported by the Cirque trackpad
RelativeReport data;

bool setup()
{
    if (!trackpad.begin()) {
        std::cout << "Cirque Pinnacle not responding!" << std::endl;
        return false;
    }
    std::cout << "CirquePinnacle/examples/linux/relative_mode\n" << std::endl;
#ifndef USE_SW_DR // if using PINNACLE_SW_DR
    std::cout << "-- Using HW DataReady pin." << std::endl;
#endif
#ifndef USE_I2C
    std::cout << "-- Using SPI interface." << std::endl;
#else
    std::cout << "-- Using I2C interface." << std::endl;
#endif
    std::cout << "\nTouch the trackpad to see the data" << std::endl;
    return true;
}

void loop()
{
    if (trackpad.available()) {
        trackpad.read(&data);
        std::cout << "Left:" << (unsigned int)(data.buttons & 1)
                  << " Right:" << (unsigned int)(data.buttons & 2)
                  << " Middle:" << (unsigned int)(data.buttons & 4)
                  << "\tX:" << (int)(data.x)
                  << "\tY:" << (int)(data.y)
                  << "\tScroll:" << (int)(data.scroll) << std::endl;
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
