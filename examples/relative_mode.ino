#include "CirquePinnacle.h"

#define ss_pin 5
#define dr_pin 6

PinnacleTouchSPI tpad = PinnacleTouchSPI(ss_pin, dr_pin);

void setup(){
    Serial.begin(9600);
    tpad.begin();
    tpad.relativeModeConfig(false, true, true);
}

void loop(){
    if (tpad.available()){
        relativeReport report = tpad.reportRelative();
        Serial.print("Left: ");
        Serial.print(report.buttons & 1);
        Serial.print("Right: ");
        Serial.print(report.buttons & 2);
        Serial.print("Middle: ");
        Serial.print(report.buttons & 4);
        Serial.print("delta X: ");
        Serial.print(report.x);
        Serial.print("delta Y: ");
        Serial.print(report.y);
        Serial.print("delta Scroll: ");
        Serial.println(report.scroll);
    }
}
