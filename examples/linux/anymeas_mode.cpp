/*
 * This example reads data from the Cirque trackpad in "anymeas mode" and prints the values.
 *
 * See documentation at https://cirquepinnacle.rtfd.io/
 */
#include <iostream>                        // cout, endl
#include <CirquePinnacle/CirquePinnacle.h> // trackpad object

#define DR_PIN 25
#define SS_PIN 0

PinnacleTouchSPI trackpad = PinnacleTouchSPI(DR_PIN, SS_PIN);

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

void compensationInit()
{
    uint8_t sweep;
    int16_t value;
    signed long accumulatedValue;
    for (uint8_t x = 0; x < variousVectors_size; ++x) {
        sweep = 0;
        accumulatedValue = 0;
        while (sweep < 5) //take 5 measurements and average them for a bit lower noise compensation value
        {
            value = trackpad.measureAdc(vectorDeterminants[x].toggle, vectorDeterminants[x].polarity);
            sweep++;
            accumulatedValue += value;
        }
        compensations[x] = accumulatedValue / 5;
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
    compensationInit();
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
