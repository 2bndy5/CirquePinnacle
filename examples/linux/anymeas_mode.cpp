#include <iostream>                        // cout, endl
#include <CirquePinnacle/CirquePinnacle.h> // trackpad  object

PinnacleTouchSPI trackpad = PinnacleTouchSPI(6, 0);

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

uint8_t variousVectors_size = sizeof(vectorDeterminants) / sizeof(measureVector);

bool setup()
{
    if (!trackpad.begin()) {
        std::cout << "Cirque Pinnacle not responding!" << std::endl;
        return false;
    }
    std::cout << "found Cirque Pinnacle!" << std::endl;
    trackpad.setDataMode(PINNACLE_ANYMEAS);
    return true;
}

void loop()
{
    for (unsigned int i = 0; i < variousVectors_size; i++) {
        int16_t measurement = trackpad.measureAdc(
            vectorDeterminants[i].toggle,
            vectorDeterminants[i].polarity);
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
