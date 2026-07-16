#include <DHT.h>

#define DHTPIN 7
#define DHTTYPE DHT22

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
byte testchar[8] = { 0b0000, 0b01110, 0b10001, 0b10101, 0b10101, 0b01110, 0b00000
};
LiquidCrystal_I2C lcd(0x27, 16, 2);
DHT dht(DHTPIN, DHTTYPE);
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
dht.begin();
Serial.print("DHT11 Sensor Test");
lcd.init();
lcd.backlight();
lcd.setCursor(0, 0);
}

void loop() {

delay(2000);
 lcd.clear();
float temp = dht.readTemperature();
float hum = dht.readHumidity();

Serial.print(temp);
Serial.print(",");
Serial.print(hum);
Serial.println("%");
 lcd.print("Nice Weather");
    lcd.write(byte(0));

    

if(isnan(temp) || isnan(hum)) {
  Serial.println("Failed to read from sensor!");
return;
}
}