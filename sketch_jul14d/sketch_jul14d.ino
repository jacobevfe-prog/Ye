#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Initialize LCD at address 0x27 (16 columns, 2 rows)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Pin Configurations
const int joyYPin = A1;  // Joystick Y-axis for aiming
const int btnPin = 2;    // Joystick select button for launching

// Custom Graphics (8x8 pixel grids)
byte birdChar[8] = {
  0b00110,
  0b01111,
  0b11101,
  0b11110,
  0b11111,
  0b01110,
  0b00000,
  0b00000
};

byte pigChar[8] = {
  0b00000,
  0b01010,
  0b11111,
  0b11011,
  0b11111,
  0b01110,
  0b01010,
  0b00000
};

// Game States
int score = 0;
bool gameOver = false;

// Slingshot / Bird variables
int aimRow = 0;          // Aiming row (0 = Top, 1 = Bottom)
float birdX = 0;         // Horizontal flight path
float birdY = 0;         // Vertical flight path
float birdVelocityY = 0; // Simulated gravity/arc trajectory
bool isFlying = false;

// Pig (Target) variables
const int pigX = 12;     
int pigY = 0;            // Target row (0 or 1)
unsigned long lastPigMove = 0;
const int pigMoveInterval = 1200; 

void setup() {
  pinMode(joyYPin, INPUT);
  pinMode(btnPin, INPUT_PUSHDOWN); // Use internal pullup for joystick switch
  
  lcd.init();
  lcd.backlight();
  
  // Register custom characters
  lcd.createChar(0, birdChar);
  lcd.createChar(1, pigChar);
  
  showStartScreen();
}

void loop() {
  if (gameOver) {
    if (digitalRead(btnPin) == LOW) {
      resetGame();
      delay(300); // Debounce
    }
    return;
  }

  // 1. Aiming (Only when bird is loaded in the slingshot)
  if (!isFlying) {
    int joyY = analogRead(joyYPin);
    if (joyY < 300) {
      aimRow = 0; // Aim High
    } else if (joyY > 700) {
      aimRow = 1; // Aim Low
    }

    // Set velocity based on current aim to calculate trajectory
    birdVelocityY = (aimRow == 0) ? 0.04 : -0.04; 

    // Launch Bird
    if (digitalRead(btnPin) == LOW) {
      isFlying = true;
      birdX = 0;
      birdY = aimRow;
      delay(200); // Debounce launch
    }
  }

  // 2. Physics & Flight update
  if (isFlying) {
    birdX += 0.8;           // Move forward horizontally
    birdY += birdVelocityY; // Factor in the gentle vertical curve
    
    // Check if flight path left the screen bounds (or flew past the pig)
    if (birdX > 14 || birdY < -0.5 || birdY > 1.5) {
      isFlying = false; // Missed target
    }

    // Collision Detection
    int roundedBirdX = round(birdX);
    int roundedBirdY = constrain(round(birdY), 0, 1);

    if (roundedBirdX == pigX && roundedBirdY == pigY) {
      score++;
      isFlying = false;
      pigY = 1 - pigY; // Swap pig row
      delay(150); 
    }
  }

  // 3. Move the target (Pig) occasionally
  if (millis() - lastPigMove > pigMoveInterval) {
    lastPigMove = millis();
    if (!isFlying && random(0, 10) > 5) { 
      pigY = 1 - pigY; // Swap pig row
    }
  }

  // 4. Render the game frame
  drawGame();
  delay(50); 
}

void drawGame() {
  lcd.clear();

  // Draw Pig at column 12
  lcd.setCursor(pigX, pigY);
  lcd.write(byte(1));

  // Draw Bird / Aiming Line / Slingshot
  if (isFlying) {
    int roundedBirdX = round(birdX);
    int roundedBirdY = constrain(round(birdY), 0, 1);
    if (roundedBirdX < 16) {
      lcd.setCursor(roundedBirdX, roundedBirdY);
      lcd.write(byte(0));
    }
  } else {
    // Show slingshot aiming position on left column
    lcd.setCursor(0, aimRow);
    lcd.write(byte(0));
    lcd.setCursor(1, aimRow);
    lcd.print(">"); // Slingshot band

    // DRAW THE AIMING LINE
    // Calculate and draw trajectory dots from Column 2 up to the Pig (Column 12)
    float tempY = aimRow;
    for (int x = 2; x < pigX; x++) {
      // Advance Y coordinate using the physics formulas
      tempY += (birdVelocityY * 1.25); // Scaling factor to match horizontal steps
      int dotY = constrain(round(tempY), 0, 1);
      
      lcd.setCursor(x, dotY);
      lcd.print("."); // Display flight path dot
    }
  }

  // Draw HUD Score
  lcd.setCursor(14, 0);
  lcd.print(score);
}

void showStartScreen() {
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("ANGRY ARDUINO");
  lcd.setCursor(0, 1);
  lcd.print("Aim Joy/Btn Launch");
}

void resetGame() {
  score = 0;
  gameOver = false;
  isFlying = false;
  aimRow = 0;
  pigY = 0;
  lcd.clear();
}