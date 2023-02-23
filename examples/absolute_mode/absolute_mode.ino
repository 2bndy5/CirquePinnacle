/*
 * This example reads data from the Cirque trackpad in "absolute mode" and prints the values.
 *
 * See documentation at https://cirquepinnacle.rtfd.io/
 */
#include "CirquePinnacle.h"

#define SS_PIN 5
#define DR_PIN 6

PinnacleTouchSPI trackpad = PinnacleTouchSPI(SS_PIN, DR_PIN);
AbsoluteReport report;

void setup() {
  while (!Serial) {
    delay(1);  // some boards need this to access USB Serial
  }
  Serial.begin(115200);
  if (!trackpad.begin()) {
    Serial.println("Cirque Pinnacle not responding!");
    while (true) {
      // hold program in infinite loop
    }
  }
  trackpad.setDataMode(PINNACLE_ABSOLUTE);
  trackpad.absoluteModeConfig(1);  // set count of z-idle packets to 1
}

void loop() {
  if (trackpad.available()) {
    trackpad.read(&report);
    Serial.print("Left: ");
    Serial.print(report.buttons & 1);
    Serial.print(" Right: ");
    Serial.print(report.buttons & 2);
    Serial.print(" Middle: ");
    Serial.print(report.buttons & 4);
    Serial.print("\tX: ");
    Serial.print(report.x);
    Serial.print("\tY: ");
    Serial.print(report.y);
    Serial.print("\tZ: ");
    Serial.println(report.z);
  }
}
