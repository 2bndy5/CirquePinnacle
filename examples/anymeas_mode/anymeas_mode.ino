/*
 * This example reads data from the Cirque trackpad in "anymeas mode" and prints the values.
 *
 * See documentation at https://cirquepinnacle.rtfd.io/
 */
#include <CirquePinnacle.h>

#define SS_PIN 2
#define DR_PIN 7

PinnacleTouchSPI trackpad(DR_PIN, SS_PIN);
// If using I2C, then use the following line (not the line above)
// PinnacleTouchI2C trackpad(DR_PIN);

typedef struct _MeasureVector {
  uint32_t toggle;
  uint32_t polarity;
} measureVector;

measureVector vectorDeterminants[] = {
  //  toggle  ,   polarity
  { 0x0000FFFF, 0x00000000 },  // toggle all x-axis bits negatively (to 0)
  { 0x0FFF0000, 0x00000000 },  // toggle all y-axis bits negatively (to 0)
  { 0x00FF00FF, 0x000000FF },  // toggle Y0-Y7 negatively (to 0) & X0-X7 positively (to 1)
  { 0x00FF00FF, 0x00FF0000 }   // toggle Y0-Y7 positively (to 1) & X0-X7 negatively (to 0)
};

const uint8_t variousVectors_size = sizeof(vectorDeterminants) / sizeof(measureVector);
int16_t compensations[variousVectors_size];

void compensate() {
  int16_t value;
  int32_t accumulatedValue;
  for (uint8_t i = 0; i < variousVectors_size; ++i) {
    uint8_t sweep = 0;
    accumulatedValue = 0;
    while (sweep < 5)  // take 5 measurements and average them for a bit lower noise compensation value
    {
      value = trackpad.measureAdc(vectorDeterminants[i].toggle, vectorDeterminants[i].polarity);
      sweep++;
      accumulatedValue += value;
    }
    compensations[i] = accumulatedValue / 5;
    Serial.print(F("compensate "));
    Serial.print(i);
    Serial.print(F(": "));
    Serial.println(compensations[i]);
  }
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
  Serial.println(F("CirquePinnacle/examples/anymeas_mode"));
  trackpad.setDataMode(PINNACLE_ANYMEAS);
  trackpad.anymeasModeConfig();
  compensate();
  Serial.println(F("starting in 5 seconds..."));
  delay(5000);
}

void loop() {
  for (uint8_t i = 0; i < variousVectors_size; i++) {
    int16_t measurement = trackpad.measureAdc(vectorDeterminants[i].toggle,
                                              vectorDeterminants[i].polarity);
    measurement -= compensations[i];
    Serial.print(F("meas"));
    Serial.print(i);
    Serial.print(F(":"));
    Serial.print(measurement);
    Serial.print(F("  \t"));
  }
  Serial.println();
}
