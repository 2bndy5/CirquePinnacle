#include "CirquePinnacle.h"

#define ss_pin 5
#define dr_pin 6

PinnacleTouchSPI tpad(ss_pin, dr_pin);

struct MeasureVector{
    unsigned int toggle;
    unsigned int polarity;
}

MeasureVector myVector[2] = {
    {toggle = 0x0000FFFF, polarity = 0x00000000},
    {toggle = 0x0FFF0000, polarity = 0x00000000},
    {toggle = 0x0000FFFF, polarity = 0x0000FFFF},
    {toggle = 0x0FFF0000, polarity = 0x0FFF0000}
};

void setup{
    Serial.begin(9600);
    tpad.begin();
    tpad.dataMode(PINNACLE_ANYMEAS);
}

void loop{
    if (tpad.available()){
        report = tpad.measureADC();
        Serial.print("Left: ");
        Serial.print(report.buttons & 1);
        Serial.print("Right: ");
        Serial.print(report.buttons & 2);
        Serial.print("Middle: ");
        Serial.print(report.buttons & 4);
        Serial.print("X-axis: ");
        Serial.print(report.x);
        Serial.print("Y-axis: ");
        Serial.print(report.y);
        Serial.print("Z magnitude: ");
        Serial.println(report.z);
    }
}
