/*
 * This example reads data from the Cirque trackpad in "anymeas mode" and prints the values.
 *
 * See documentation at https://cirquepinnacle.rtfd.io/
 */
#include "CirquePinnacle.h"

#define SS_PIN 5
#define DR_PIN 6

PinnacleTouchSPI trackpad = PinnacleTouchSPI(SS_PIN, DR_PIN);

typedef struct _MeasureVector {
  unsigned long toggle;
  unsigned long polarity;
} measureVector;

measureVector vectorDeterminants[] = {
  // {toggle, polarity}
  { 0x0000FFFF, 0x00000000 },  // toggle all x-axis bits negatively (to 0)
  { 0x0FFF0000, 0x00000000 },  // toggle all y-axis bits negatively (to 0)
  { 0x00FF00FF, 0x000000FF },  // toggle Y0-Y7 negatively (to 0) & X0-X7 positively (to 1)
  { 0x00FF00FF, 0x00FF0000 }   // toggle Y0-Y7 positively (to 1) & X0-X7 negatively (to 0)
};

uint8_t variousVectors_size = sizeof(vectorDeterminants) / sizeof(measureVector);

void setup() {
  Serial.begin(9600);
  if (trackpad.begin()) {
    Serial.println("found Cirque Pinnacle!");
  } else {
    Serial.println("Cirque Pinnacle not responding!");
  }
  trackpad.setDataMode(PINNACLE_ANYMEAS);
}

void loop() {
  for (uint8_t i = 0; i < variousVectors_size; i++) {
    int16_t measurement = trackpad.measureAdc(
      vectorDeterminants[i].toggle,
      vectorDeterminants[i].polarity);
    Serial.print("meas");
    Serial.print(i);
    Serial.print(": ");
    Serial.print(measurement);
    Serial.print("\t");
  }
  Serial.println();
}
