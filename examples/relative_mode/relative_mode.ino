#include <Arduino.h>
#include "CirquePinnacle.h"

#define ss_pin 5
#define dr_pin 6

PinnacleTouchSPI tpad = PinnacleTouchSPI(dr_pin, ss_pin);
RelativeReport report;

void setup() {
  while (!Serial) {
    delay(1);  // some boards need this to access USB Serial
  }
  Serial.begin(9600);
  if (tpad.begin()) {
    Serial.println("found Cirque Pinnacle!");
  }
  else {
    Serial.println("Cirque Pinnacle not responding!");
  }
}

void loop() {
  if (tpad.available()) {
    tpad.read(&report);
    Serial.print("Left: ");
    Serial.print(report.buttons & 1);
    Serial.print(" Right: ");
    Serial.print(report.buttons & 2);
    Serial.print(" Middle: ");
    Serial.print(report.buttons & 4);
    Serial.print("\tdelta X: ");
    Serial.print(report.x);
    Serial.print("\tdelta Y: ");
    Serial.print(report.y);
    Serial.print("\tdelta Scroll: ");
    Serial.println(report.scroll);
  }
}
