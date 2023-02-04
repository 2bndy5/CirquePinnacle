/**
 * AnyMeas mode basic example of PinnacleTouch API
 */

#include <Arduino.h>
#include "CirquePinnacle.h"

#define ss_pin 5
#define dr_pin 6

PinnacleTouchSPI tpad = PinnacleTouchSPI(ss_pin, dr_pin);

typedef struct _MeasureVector {
  unsigned long toggle;
  unsigned long polarity;
} measureVector;

measureVector vectorDetirminants[] = {
  // {toggle, polarity}
  { 0x0000FFFF, 0x00000000 },  // toggle all x-axis bits negatively (to 0)
  { 0x0FFF0000, 0x00000000 },  // toggle all y-axis bits negatively (to 0)
  { 0x00FF00FF, 0x000000FF },  // toggle Y0-Y7 negatively (to 0) & X0-X7 positively (to 1)
  { 0x00FF00FF, 0x00FF0000 }   // toggle Y0-Y7 positively (to 1) & X0-X7 negatively (to 0)
};

uint8_t variousVectors_size = sizeof(vectorDetirminants) / sizeof(measureVector);

void setup() {
  Serial.begin(9600);
  if (tpad.begin()) {
    Serial.println("found Cirque Pinnacle!");
  } else {
    Serial.println("Cirque Pinnacle not responding!");
  }
  tpad.setDataMode(PINNACLE_ANYMEAS);
}

void loop() {
  for (uint8_t i = 0; i < variousVectors_size; i++) {
    int16_t measurement = tpad.measureAdc(
      vectorDetirminants[i].toggle,
      vectorDetirminants[i].polarity);
    Serial.print("meas");
    Serial.print(i);
    Serial.print(": ");
    Serial.print(measurement);
    Serial.print("\t");
  }
  Serial.println();
}
