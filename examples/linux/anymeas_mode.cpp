/*
 * This example reads data from the Cirque trackpad in "anymeas mode" and prints the values.
 *
 * See documentation at https://cirquepinnacle.rtfd.io/
 */
#include <iostream>                        // cout, endl
#include <unistd.h>                        // sleep()
#include <CirquePinnacle/CirquePinnacle.h> // trackpad object

#ifdef PINNACLE_DRIVER_mraa
    #define DR_PIN 22 // GPIO25
#else
    #define DR_PIN 25
#endif
#define SS_PIN 0

#ifndef USE_I2C
PinnacleTouchSPI trackpad(DR_PIN, SS_PIN);
#else // If using I2C, then use the following line (not the line above)
PinnacleTouchI2C trackpad(DR_PIN);
#endif

typedef struct _MeasureVector
{
    unsigned long toggle;
    unsigned long polarity;
} measureVector;

measureVector vectorDeterminants[] = {
    // {toggle, polarity}
    {0x0000FFFF, 0x00000000}, // toggle all x-axis bits negatively (to 0)
    {0x0FFF0000, 0x00000000}, // toggle all y-axis bits negatively (to 0)
    {0x00FF00FF, 0x000000FF}, // toggle Y0-Y7 negatively (to 0) & X0-X7 positively (to 1)
    {0x00FF00FF, 0x00FF0000}  // toggle Y0-Y7 positively (to 1) & X0-X7 negatively (to 0)
};

const uint8_t variousVectors_size = sizeof(vectorDeterminants) / sizeof(measureVector);
int16_t compensations[variousVectors_size];

void compensate()
{
    signed long accumulatedValue;
    for (uint8_t i = 0; i < variousVectors_size; ++i) {
        uint8_t sweep = 0;
        accumulatedValue = 0;
        while (sweep < 5) // take 5 measurements and average them for a bit lower noise compensation value
        {
            int16_t value = trackpad.measureAdc(vectorDeterminants[i].toggle, vectorDeterminants[i].polarity);
            sweep++;
            accumulatedValue += value;
        }
        compensations[i] = accumulatedValue / 5;
        std::cout << "compensation " << (unsigned int)i << ": " << compensations[i] << std::endl;
    }
}

bool setup()
{
    if (!trackpad.begin()) {
        std::cout << "Cirque Pinnacle not responding!" << std::endl;
        return false;
    }
    std::cout << "CirquePinnacle/examples/linux/anymeas_mode" << std::endl;
    trackpad.setDataMode(PINNACLE_ANYMEAS);
    trackpad.anymeasModeConfig();
#ifndef USE_SW_DR // if using PINNACLE_SW_DR
    std::cout << "-- Using HW DataReady pin." << std::endl;
#endif
#ifndef USE_I2C
    std::cout << "-- Using SPI interface." << std::endl;
#else
    std::cout << "-- Using I2C interface." << std::endl;
#endif
    compensate();

    for (uint8_t i = 5; i; --i) {
        std::cout << "starting in " << (unsigned int)i << "second" << (i < 1 ? 's' : ' ') << '\r';
        sleep(1);
    }
    std::cout << std::endl;
    return true;
}

void loop()
{
    for (unsigned int i = 0; i < variousVectors_size; i++) {
        int16_t measurement = trackpad.measureAdc(
            vectorDeterminants[i].toggle,
            vectorDeterminants[i].polarity);
        measurement -= compensations[i];
        std::cout << "meas" << i << ": " << measurement << "\t";
    }
    std::cout << std::endl;
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
