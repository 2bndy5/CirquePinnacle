/*
 * This example reads data from the Cirque trackpad in "absolute mode" and prints the values.
 *
 * See documentation at https://cirquepinnacle.rtfd.io/
 */
#include <math.h>  // sqrt(), pow(), atan2()
#include <CirquePinnacle.h>

#define SS_PIN 2
#define DR_PIN 7

PinnacleTouchSPI trackpad(DR_PIN, SS_PIN);
// If using I2C, then use the following line (not the line above)
// PinnacleTouchI2C trackpad(DR_PIN);

// an object to hold data reported by the Cirque trackpad
AbsoluteReport data;

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
  Serial.println(F("CirquePinnacle/examples/absolute_mode"));
  trackpad.setDataMode(PINNACLE_ABSOLUTE);
  trackpad.absoluteModeConfig(1);  // set count of z-idle packets to 1

  // pinMode() is already called by trackpad.begin()
  attachInterrupt(digitalPinToInterrupt(DR_PIN), interruptHandler, RISING);

  Serial.println(F("\n*** Enter 'M' to measure and print raw data."));
  Serial.println(F("*** Enter 'T' to measure and print trigonometric calculations.\n"));
  Serial.println(F("Touch the trackpad to see the data."));
}

/*
Showing all the Serial output below will slow down the board's ability to
read() data from the trackpad in a timely manner (resulting in data loss).
Use this compile-time definition to switch between printing
raw data (false) or trigonometry data (true)
*/
bool onlyShowTrigVals = false;

#if defined(M_PI) && !defined(PI)
#define PI M_PI
#endif
#ifndef PI
#define PI 3.14159
#endif

void loop() {
  // using `interruptHandler()` to update `isDataReady`
  if (isDataReady) {
    isDataReady = false;  // reset our IRQ flag
    trackpad.read(&data);

    // datasheet recommends clamping the axes value to reliable range
    if (data.z) {  // only clamp values if Z axis is not idle.

      data.x = data.x > 1920 ? 1920 : (data.x < 128 ? 128 : data.x);  // 128 <= x <= 1920
      data.y = data.y > 1472 ? 1472 : (data.y < 64 ? 64 : data.y);    //  64 <= y <= 1472
    }

    if (!onlyShowTrigVals) {
      // print raw data from the trackpad
      Serial.print(F("B1:"));
      Serial.print(data.buttons & 1);
      Serial.print(F(" B2:"));
      Serial.print(data.buttons & 2);
      Serial.print(F(" B3:"));
      Serial.print(data.buttons & 4);
      Serial.print(F("\tX:"));
      Serial.print(data.x);
      Serial.print(F("\tY:"));
      Serial.print(data.y);
      Serial.print(F("\tZ:"));
      Serial.println(data.z);
    } else {
      // print trigonometric data from the trackpad
      if (!data.z) {  // only compute angle and radius if touching (or near) the sensor
        Serial.println(F("Idling"));
      } else {
        // coordinates assume axes have been clamped to recommended ranges
        double coord_x = (int16_t)(data.x) - 960;
        double coord_y = (int16_t)(data.y) - 736;  // NOTE: y-axis is inverted by default
        double radius = sqrt(pow(coord_x, 2) + pow(coord_y, 2));
        // angle (in degrees) ranges (-180, 180]
        double angle = atan2(coord_y, coord_x) * 180 / PI;

        // This Serial output is meant to be compatible with the Arduino IDE Serial Plotter applet.
        Serial.print(F("angle:"));
        Serial.print(angle);
        Serial.print(F(",radius:"));
        Serial.println(radius);
      }
    }
  }  // end if trackpad.available()

  while (Serial.available()) {
    char input = Serial.read();
    if (input == 't' || input == 'T') {
      onlyShowTrigVals = true;
    } else if (input == 'm' || input == 'M') {
      onlyShowTrigVals = false;
    }
  }
}
