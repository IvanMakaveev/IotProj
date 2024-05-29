#include "Adafruit_VL53L0X.h"
Adafruit_VL53L0X lox = Adafruit_VL53L0X();

void setup() {
  Serial.begin(9600);

  while (!Serial) delay(1);

  Wire.begin(16, 17);

  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }

  lox.startRangeContinuous();
}

void loop() {
  if (lox.isRangeComplete()) {
    uint16_t range = lox.readRange(); // in mm
  }
}