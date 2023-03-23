#include <Mouse.h>
#include <CirquePinnacle.h>

#define SS_PIN 2
#define DR_PIN 7

PinnacleTouchSPI trackpad = PinnacleTouchSPI(DR_PIN, SS_PIN);
// If using I2C, then use the following line (not the line above)
// PinnacleTouchI2C trackpad = PinnacleTouchI2C(DR_PIN);

RelativeReport report;

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    // wait here to ensure the trackpad was initialized via Serial Monitor
  }

  if (!trackpad.begin()) {
    Serial.println("Cirque Pinnacle not responding!");
    while (true) {
      // hold program in infinite loop
    }
  }
  Serial.println("CirquePinnacle/examples/usb_mouse");
  // By default, the trackpad is configured to use relative mode with
  // tap detection enabled. This config fits nicely with Mouse HID reports!
}

void loop() {

  if (trackpad.available()) {  // is there new data?

    // save buttons' previous state before getting updates
    uint8_t prevButtonStates = report.buttons; // for edge detection

    trackpad.read(&report);  // get new data

    uint8_t buttonsChanged = prevButtonStates ^ report.buttons;
    if (buttonsChanged) {
      uint8_t toggledOff = buttonsChanged ^ (report.buttons & buttonsChanged);
      uint8_t toggledOn = buttonsChanged ^ toggledOff;
      if (toggledOn) {
        Mouse.press(toggledOn);
      }
      if (toggledOff) {
        Mouse.release(toggledOff);
      }
    }

    if (report.x || report.y || report.scroll) {
      // invert the x-axis, use the others as is
      Mouse.move(report.x * -1, report.y, report.scroll);
    }
  }
}
