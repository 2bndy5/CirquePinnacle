/*
 * This example reads data from the Cirque trackpad in "relative mode" and prints the values.
 *
 * See documentation at https://cirquepinnacle.rtfd.io/
 */
#include <iostream>                        // cout, endl
#include <CirquePinnacle/CirquePinnacle.h> // trackpad object

#define DR_PIN 25 // GPIO25
#define SS_PIN 0

#ifndef USE_I2C
PinnacleTouchSPI trackpad(DR_PIN, SS_PIN);
#else // If using I2C, then use the following line (not the line above)
PinnacleTouchI2C trackpad(DR_PIN);
#endif

// an object to hold data reported by the Cirque trackpad
RelativeReport data;

// track the interrupts with our own IRQ flag
volatile bool isDataReady = false;

void interruptHandler()
{
    isDataReady = true;
}

bool setup()
{
    if (!trackpad.begin()) {
        std::cout << "Cirque Pinnacle not responding!" << std::endl;
        return false;
    }
    std::cout << "CirquePinnacle/examples/linux/relative_mode\n"
              << std::endl;

    // pull in arduino-like namespace
    namespace arduino = cirque_pinnacle_arduino_wrappers;
    // setup the interrupt handler
    arduino::attachInterrupt(DR_PIN, &interruptHandler, arduino::RISING);

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
    // using `interruptHandler()` to update `isDataReady`
    if (isDataReady) {
        // assert(isDataReady == trackpad.available());
        isDataReady = false; // reset our IRQ flag
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
