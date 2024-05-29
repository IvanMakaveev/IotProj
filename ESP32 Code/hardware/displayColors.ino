#include <NeoPixelBus.h>

const uint16_t PixelCount = 16; // this example assumes 4 pixels, making it smaller will cause a failure
const uint8_t PixelPin = 2;  // make sure to set this to the correct pin, ignored for Esp8266
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount, PixelPin);

#define Cred     RgbColor(255, 0, 0)
#define Cpink    RgbColor(255, 0, 128)
#define Clila    RgbColor(255, 0, 255)
#define Cviolet  RgbColor(128, 0, 255)
#define Cblue    RgbColor(0, 0, 255)

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(1);
  }

  strip.Begin();
  strip.Show();
}

void loop() {
  for(int i = 0; i < PixelCount; i++) {
      strip.SetPixelColor(i, Cblue);
      strip.Show();
  }
}
