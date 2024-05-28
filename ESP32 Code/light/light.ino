#include "Adafruit_Si7021.h"
#include <LiquidCrystal_I2C.h>
#include <NeoPixelBus.h>

const uint16_t PixelCount = 16; // this example assumes 4 pixels, making it smaller will cause a failure
const uint8_t PixelPin = 2;  // make sure to set this to the correct pin, ignored for Esp8266

bool enableHeater = false;
uint8_t loopCnt = 0;
float initTemp = 0;
float diff = 0;

NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount, PixelPin);

Adafruit_Si7021 sensor = Adafruit_Si7021();
LiquidCrystal_I2C lcd(0x3F,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

#define Cred     RgbColor(255, 0, 0)
#define Cpink    RgbColor(255, 0, 128)
#define Clila    RgbColor(255, 0, 255)
#define Cviolet  RgbColor(128, 0, 255)
#define Cblue    RgbColor(0, 0, 255)
#define Cmblue   RgbColor(0, 128, 255)
#define Ccyan    RgbColor(0, 255, 255)
#define Cgreen   RgbColor(0, 255, 0)
#define Cyellow  RgbColor(255, 255, 0)
#define Corange  RgbColor(255, 100, 0)
#define Cwhite   RgbColor(255, 255, 255)
#define Cblack   RgbColor(0) 

void recalculate(float desired){
  init = sensor.readTemperature();
  diff = (init - desired) / 3;
} 

void setup() {
  Serial.begin(115200);
  Wire.begin(16, 17);
  // wait for serial port to open
  while (!Serial) {
    delay(10);
  }

  Serial.println("Si7021 test!");
  
  if (!sensor.begin()) {
    Serial.println("Did not find Si7021 sensor!");
    while (true)
      ;
  }

  Serial.print("Found model ");
  switch(sensor.getModel()) {
    case SI_Engineering_Samples:
      Serial.print("SI engineering samples"); break;
    case SI_7013:
      Serial.print("Si7013"); break;
    case SI_7020:
      Serial.print("Si7020"); break;
    case SI_7021:
      Serial.print("Si7021"); break;
    case SI_UNKNOWN:
    default:
      Serial.print("Unknown");
  }
  Serial.print(" Rev(");
  Serial.print(sensor.getRevision());
  Serial.print(")");
  Serial.print(" Serial #"); Serial.print(sensor.sernum_a, HEX); Serial.println(sensor.sernum_b, HEX);

  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();

  strip.Begin();
  strip.Show();
}

void loop() {
  lcd.setCursor(0,0);

  lcd.print("Temp: ");
  float temp = sensor.readTemperature();
  lcd.print(temp, 1);

  //if we receive a query from user
  //float desired = getQuery()
  recalculate(desired);

  for(int i = 0; i < PixelCount; i++) {
    if (temp < desired) {
      strip.SetPixelColor(i, Cblue);
      strip.Show();
    }
    else if (temp < desired + diff) {
      strip.SetPixelColor(i, Cviolet);
      strip.Show();
    }
    else if (temp < desired + 2*diff) {
      strip.SetPixelColor(i, Clila);
      strip.Show();
    }
    else if (temp < desired + 3*diff) {
      strip.SetPixelColor(i, Cpink);
      strip.Show();
    }
    else {
      strip.SetPixelColor(i, Cred);
      strip.Show();
    }
  }

  // Toggle heater enabled state every 30 seconds
  // An ~1.8 degC temperature increase can be noted when heater is enabled
  if (++loopCnt == 30) {
    enableHeater = !enableHeater;
    sensor.heater(enableHeater);
    Serial.print("Heater Enabled State: ");
    if (sensor.isHeaterEnabled())
      Serial.println("ENABLED");
    else
      Serial.println("DISABLED");
       
    loopCnt = 0;
  }
}
