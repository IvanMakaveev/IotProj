#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 environmentSensor;

void setup() {
    Serial.begin(9600); // Console output
    while(!Serial); // Serial connection wait 
    Wire.begin(16, 17); // Begin SDA SCL Wires
    environmentSensor.begin(0x76);  // Connect environment sensor
}


void loop() { 
    printValues();
    delay(1000);
}


void printValues() {
    //Serial.print(environmentSensor.readTemperature());
    //Serial.println(environmentSensor.readPressure() / 100.0F);
    //Serial.print(environmentSensor.readAltitude(SEALEVELPRESSURE_HPA));
    //Serial.print(environmentSensor.readHumidity());
}