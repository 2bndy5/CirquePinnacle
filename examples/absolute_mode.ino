#include "CirquePinnacle.h"

#define ss_pin 5
#define dr_pin 6

PinnacleTouchSPI tpad(ss_pin, dr_pin);

void setup{
    Serial.begin(9600);
    tpad.begin();
    tpad.dataMode(PINNACLE_ABSOLUTE);
    tpad.absoluteModeConfig(zIdleCount=1);
}

void loop{
    if (tpad.available()){
        RelativeReport report = tpad.relativeReport();
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
