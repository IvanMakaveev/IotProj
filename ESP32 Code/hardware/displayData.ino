#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);

void setup()
{
  Wire.begin(16,17);
  lcd.init();

  lcd.backlight();
  lcd.setCursor(3,0);
  lcd.print("AGD 145");
  lcd.setCursor(5,3);
  lcd.print("BMK 4PS");
}


void loop()
{
  
}