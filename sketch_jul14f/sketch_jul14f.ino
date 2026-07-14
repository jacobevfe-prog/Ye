#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Initialize the LCD (Address 0x27, 16 columns, 2 rows)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Pin Definitions
const int JOY_X = A0;
const int JOY_SW = 2;

// Game State Variables
int playerScore = 0;
int dealerScore = 0;
bool gameOver = false;
bool playerTurn = true;

// Wagering Variables
int totalPoints = 100;
int currentBet = 10;

void setup() {
  pinMode(JOY_SW, INPUT_PULLUP);
  
  lcd.init();
  lcd.backlight();
  
  // Seed random generator using an unconnected analog pin
  randomSeed(analogRead(A3));
  
  showIntro();
}

void loop() {
  if (totalPoints <= 0) {
    lcd.clear();
    lcd.print("   GAME OVER!   ");
    lcd.setCursor(0, 1);
    lcd.print("Out of points!");
    delay(3000);
    totalPoints = 100; // Reset bankroll
    showIntro();
    return;
  }

  // 1. Wagering Phase
  getWager();

  // 2. Play Phase
  resetGame();

  while (!gameOver) {
    if (playerTurn) {
      lcd.setCursor(0, 0);
      lcd.print("You:" + String(playerScore) + "  Bet:" + String(currentBet) + " ");
      lcd.setCursor(0, 1);
      lcd.print("<-Hit   Stand->");

      int xVal = analogRead(JOY_X);

      // Joystick tilted Left (HIT)
      if (xVal < 200) {
        int card = drawCard();
        playerScore += card;
        
        lcd.clear();
        lcd.print("Drew a " + String(card));
        delay(1200);
        
        if (playerScore > 21) {
          lcd.clear();
          lcd.print("Busted! " + String(playerScore));
          lcd.setCursor(0, 1);
          lcd.print("Lost " + String(currentBet) + " pts!");
          totalPoints -= currentBet;
          gameOver = true;
        }
        delay(300); // Prevent double registering
      } 
      // Joystick tilted Right (STAND)
      else if (xVal > 800) {
        playerTurn = false;
        lcd.clear();
        lcd.print("Standing at " + String(playerScore));
        delay(1500);
      }
    } else {
      // Dealer's Turn
      lcd.clear();
      lcd.print("Dealer's turn...");
      delay(1000);

      // Dealer hits on 16 and under, stands on 17
      while (dealerScore < 17) {
        int card = drawCard();
        dealerScore += card;
        lcd.clear();
        lcd.print("Dealer draws " + String(card));
        lcd.setCursor(0, 1);
        lcd.print("Score: " + String(dealerScore));
        delay(1500);
      }

      // Determine Winner
      lcd.clear();
      if (dealerScore > 21) {
        lcd.print("Dealer Busted!");
        lcd.setCursor(0, 1);
        lcd.print("Won " + String(currentBet) + " pts!");
        totalPoints += currentBet;
      } else if (playerScore > dealerScore) {
        lcd.print("Y:" + String(playerScore) + " vs D:" + String(dealerScore));
        lcd.setCursor(0, 1);
        lcd.print("Won " + String(currentBet) + " pts!");
        totalPoints += currentBet;
      } else if (dealerScore > playerScore) {
        lcd.print("Y:" + String(playerScore) + " vs D:" + String(dealerScore));
        lcd.setCursor(0, 1);
        lcd.print("Lost " + String(currentBet) + " pts!");
        totalPoints -= currentBet;
      } else {
        lcd.print("Y:" + String(playerScore) + " vs D:" + String(dealerScore));
        lcd.setCursor(0, 1);
        lcd.print("Push! (Tie)");
        // totalPoints remains unchanged
      }
      
      gameOver = true;
      delay(2500);
    }
  }

  // 3. Round End Phase
  lcd.clear();
  lcd.print("Bank: " + String(totalPoints) + " pts");
  lcd.setCursor(0, 1);
  lcd.print("Press to play");
  
  while (digitalRead(JOY_SW) == HIGH) {
    // Wait for button press to start next round
  }
  delay(300); // Debounce
}

// Generates a random card value
int drawCard() {
  int card = random(1, 14); // 1 to 13
  if (card > 10) {
    return 10; // Jack, Queen, King are worth 10
  }
  if (card == 1) {
    return 11; // Treat Aces as 11 for simplicity
  }
  return card;
}

// Allows the player to adjust their bet using the Joystick
void getWager() {
  currentBet = 10; // Reset default bet to 10
  lcd.clear();
  
  while (digitalRead(JOY_SW) == HIGH) {
    lcd.setCursor(0, 0);
    lcd.print("Bank: " + String(totalPoints) + " pts");
    lcd.setCursor(0, 1);
    lcd.print("Wager: < " + String(currentBet) + " >  ");

    int xVal = analogRead(JOY_X);

    // Joystick Left - Decrease Bet
    if (xVal < 200) {
      if (currentBet > 10) {
        currentBet -= 10;
      }
      delay(200); // Navigation delay
    }
    // Joystick Right - Increase Bet
    else if (xVal > 800) {
      if (currentBet < totalPoints) {
        currentBet += 10;
      }
      delay(200); // Navigation delay
    }
  }
  
  lcd.clear();
  lcd.print("Bet Locked: " + String(currentBet));
  delay(1200);
}

void resetGame() {
  lcd.clear();
  lcd.print("Shuffling...");
  delay(1000);
  
  // Deal initial cards
  playerScore = drawCard() + drawCard();
  dealerScore = drawCard(); // Show one dealer card
  
  lcd.clear();
  lcd.print("Dealer shows: " + String(dealerScore));
  delay(2000);
  
  playerTurn = true;
  gameOver = false;
}

void showIntro() {
  lcd.clear();
  lcd.print("  Blackjack 21  ");
  lcd.setCursor(0, 1);
  lcd.print("  Press Button  ");
  while (digitalRead(JOY_SW) == HIGH) {
    // Wait for user to press the button to start
  }
  delay(300); 
}
