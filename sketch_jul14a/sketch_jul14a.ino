const int redPin =9;
const int bluePin =10;
const int yellowPin= 11;
const int whitePin = 12;
const int joyPinx = A0;
const int joyPiny =A1;
const int CENTER_LOW  = 505;
const int CENTER_HIGH = 520;

void setup() {
  // put your setup code here, to run once:
pinMode(redPin, OUTPUT);
pinMode(bluePin, OUTPUT);
pinMode(yellowPin, OUTPUT);
pinMode(whitePin, OUTPUT);
}

void loop() {
 int xVal = analogRead(joyPinx);
 int yVal = analogRead(joyPiny);
 int redBright =0;
int blueBright =0;
int yellowBright= 0;
int whiteBright = 0;

if (yVal < CENTER_LOW){
  redBright = map(yVal, CENTER_LOW, 0, 0, 255);
}
else if (yVal > CENTER_HIGH) {
    // Joystick moved DOWN: values rise from ~512 towards 1023
    blueBright = map(yVal, CENTER_HIGH, 1023, 0, 255);
  }

  // --- X-AXIS CONTROL (Left / Right) ---
  if (xVal < CENTER_LOW) {
    // Joystick moved LEFT: values drop from ~512 towards 0
    yellowBright = map(xVal, CENTER_LOW, 0, 0, 255);
  } 
  else if (xVal > CENTER_HIGH) {
    // Joystick moved RIGHT: values rise from ~512 towards 1023
    whiteBright = map(xVal, CENTER_HIGH, 1023, 0, 255);
  }

  // Write scaled brightness parameters to the PWM pins
  analogWrite(redPin, redBright);
  analogWrite(bluePin, blueBright);
  analogWrite(yellowPin, yellowBright);
  analogWrite(whitePin, whiteBright);

  delay(10); 

}

