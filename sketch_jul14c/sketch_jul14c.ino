#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
#define joyX A0
#define joyY A1
byte testchar[8] = { 0b0000, 0b01110, 0b10001, 0b10101, 0b10101, 0b01110, 0b00000
};
byte smiley[8] {
  0b00000, 0b00000, 0b01010, 0b00000, 0b10001, 0b01110, 0b00000, 0b00000
};
byte customChar[] = {
  B00100,
  B00010,
  B00001,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};
int xValue, yValue;
void setup() {
  // put your setup code here, to run once:
lcd.init();
lcd.backlight();
lcd.createChar(0, customChar);
lcd.setCursor(0, 0);
lcd.print("HI");
lcd.write(byte(0));
lcd.print("hello");
}

void loop() {
xValue =  analogRead(joyX);
 yValue = analogRead(joyY);
lcd.setCursor(0,0); 
lcd.print("X");
lcd.print(xValue);//Displaying  xValue on top 
lcd.setCursor(0,1);
lcd.print("Y ");
lcd.print(yValue);//Displaying yValue on bottom  
delay(500);
lcd.clear();
}
