/*
 * This example reads data from the Cirque trackpad in "relative mode" and prints the values.
 *
 * See documentation at https://cirquepinnacle.rtfd.io/
 */
#include <CirquePinnacle.h>

#define SS_PIN 2
#define DR_PIN 7

PinnacleTouchSPI trackpad(DR_PIN, SS_PIN);
// If using I2C, then use the following line (not the line above)
// PinnacleTouchI2C trackpad(DR_PIN);

// an object to hold data reported by the Cirque trackpad
RelativeReport data;

// track the interrupts with our own IRQ flag
volatile bool isDataReady = false;

void interruptHandler() {
  isDataReady = true;
}

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    // wait till Serial monitor is opened
  }
  if (!trackpad.begin()) {
    Serial.println(F("Cirque Pinnacle not responding!"));
    while (true) {
      // hold program in infinite loop
    }
  }
  Serial.println(F("CirquePinnacle/examples/relative_mode"));
  trackpad.setDataMode(PINNACLE_RELATIVE);
  trackpad.relativeModeConfig();  // uses default config

  // pinMode() is already called by trackpad.begin()
  attachInterrupt(digitalPinToInterrupt(DR_PIN), interruptHandler, FALLING);

  Serial.println(F("Touch the trackpad to see the data."));
}

void loop() {

  // using `interruptHandler()` to update `isDataReady`
  if (isDataReady) {
    isDataReady = false;  // reset our IRQ flag
    trackpad.read(&data);
    Serial.print(F("Left:"));
    Serial.print(data.buttons & 1);
    Serial.print(F(" Right:"));
    Serial.print(data.buttons & 2);
    Serial.print(F(" Middle:"));
    Serial.print(data.buttons & 4);
    Serial.print(F("\tX:"));
    Serial.print(data.x);
    Serial.print(F("\tY:"));
    Serial.print(data.y);
    Serial.print(F("\tScroll:"));
    Serial.println(data.scroll);
  }
}
