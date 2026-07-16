#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
const int trigPin = 10;
int pos = 0;
const int echoPin = 9;
Servo myservo; 
byte testchar[8] = { 0b0000, 0b01110, 0b10001, 0b10101, 0b10101, 0b01110, 0b00000
};
long duration;
long distance;

void setup() {
  myservo.attach(A0);
 pinMode(echoPin, INPUT);
 pinMode(trigPin, OUTPUT);
Serial.begin(9600);
lcd.init();
lcd.backlight();
lcd.setCursor(0, 0);



}

void loop() {
  // put your main code here, to run repeatedly:
  measureDistance();
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(35);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(35);                       // waits 15ms for the servo to reach the position
  }

}

float measureDistance() {
  //fire pulse
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.0343 / 2.0;
  Serial.println(distance);

  if (distance <= 5 ) {
    
    lcd.print("Something is very close");
    lcd.write(byte(0));

  }
  
  else if (distance <= 20) {
    
    lcd.print("Something is near");
    lcd.write(byte(0));

  }
  else if (distance <= 30) {
  
    lcd.print("Something is around");
    lcd.write(byte(0));

  }
  delay(100);
}
  
