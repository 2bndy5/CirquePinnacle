#include <Arduino.h>
#include <Mouse.h>
#include "CirquePinnacle.h"

#define BACKWARD_BUTTON_MASK 0x10
#define FORWARD_BUTTON_MASK 0x40

// to send special mouse buttons' states (backward/forward buttons),
// uncomment the following defines, & make sure their number values
// match the digital input pins they're connected to.
// #define BACKWARD_BUTTON_PIN 3
// #define FORWARD_BUTTON_PIN 4

class MouseHID {
    MouseHID(void) {
      static HIDSubDescriptor node(_hidReportDescriptor, sizeof(_hidReportDescriptor));
      HID().AppendDescriptor(&node);
    }
    void sendReport(relativeReport* r) {
      HID().SendReport(1, r, 4);
    }
};

#define ss_pin 5
#define dr_pin 6

// using a custom Mouse HID class that exposes the HID::sendReport()
MouseHID* mouse = new MouseHID;

PinnacleTouchSPI tpad = PinnacleTouchSPI(dr_pin, ss_pin);
RelativeReport report;
bool hasNewReport = false;

void setup() {
  if (tpad.begin()) {
    Serial.println("found Cirque Pinnacle!");
  }
  else {
    Serial.println("Cirque Pinnacle not responding!");
  }
#ifdef FORWARD_BUTTON_PIN
  pinMode(FORWARD_BUTTON_PIN, INPUT_PULLUP);
#endif
#ifdef BACKWARD_BUTTON_PIN
  pinMode(BACKWARD_BUTTON_PIN, INPUT_PULLUP);
#endif
}

void loop() {
  relativeReport prevReportState = report;
  if (tpad.available()) { // is there new data?
    tpad.reportRelative(&report); // get new data
    hasNewReport = true;
  }
  else {
    // clear previous forward/backward buttons' states
    report.buttons &= 7;
  }
#ifdef FORWARD_BUTTON_PIN
  report.buttons |= digitalRead(FORWARD_BUTTON_PIN) * FORWARD_BUTTON_MASK;
#endif
#ifdef BACKWARD_BUTTON_PIN
  report.buttons |= digitalRead(BACKWARD_BUTTON_PIN) * BACKWARD_BUTTON_MASK;
#endif
  if (prevReportState.buttons != report.buttons || hasNewReport) {
    prevReportState = report;
    mouse->sendReport(&report);
    hasNewReport = false;
  }
}
