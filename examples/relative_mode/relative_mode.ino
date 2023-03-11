/*
 * This example reads data from the Cirque trackpad in "relative mode" and prints the values.
 *
 * See documentation at https://cirquepinnacle.rtfd.io/
 */
#include "CirquePinnacle.h"

#define SS_PIN 5
#define DR_PIN 6

PinnacleTouchSPI trackpad = PinnacleTouchSPI(DR_PIN, SS_PIN);
RelativeReport report;

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
  Serial.println("CirquePinnacle/examples/relative_mode");
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
    Serial.print("\tdelta X: ");
    Serial.print(report.x);
    Serial.print("\tdelta Y: ");
    Serial.print(report.y);
    Serial.print("\tdelta Scroll: ");
    Serial.println(report.scroll);
  }
}
