#include "CirquePinnacle.h"

#define ss_pin 5
#define dr_pin 6

PinnacleTouchSPI tpad = PinnacleTouchSPI(ss_pin, dr_pin);

void setup(){
    Serial.begin(9600);
    tpad.begin();
    tpad.setDataMode(PINNACLE_ABSOLUTE);
    tpad.absoluteModeConfig(1); // only 1 z-idle packet sent (default is 30)
}

void loop(){
    if (tpad.available()){
        absoluteReport report = tpad.relativeReport();
        Serial.print("button1: ");
        Serial.print(report.buttons & 1);
        Serial.print("button2: ");
        Serial.print(report.buttons & 2);
        Serial.print("button3: ");
        Serial.print(report.buttons & 4);
        Serial.print("X-axis: ");
        Serial.print(report.x);
        Serial.print("Y-axis: ");
        Serial.print(report.y);
        Serial.print("Z magnitude: ");
        Serial.println(report.z);
    }
}
