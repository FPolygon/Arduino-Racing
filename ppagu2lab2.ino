/*
   1- Author: Francis Pagulayan (as shown in Blackboard) - 665900279
   2- Lab: Lab 2 - The LCD 
   3- Description: This code has 2 buttons and an 2x16 LCD. It will initialize the LCD with greetings to layer 1 and 2. 
   On a button push, each player will get their respective lines on the screen and every button press and release will 
   increment a symbol to be added to each respective line. When a line is filled, that line is cleared and the player goes to round 2
   and repeats filling the line. When round 2 is completed, that line will terminate with the special winning character and after
   a delay, return to the title screen. 
   4- LCD Pins: List what each of the following LCD pins are for, i.e. “D4: Data pin 4”. (Read the prelab)
     VSS(GND): Pin for ground
     VDD(+5V): Pin for 5V
     V0: Contrast adjustment pin
     RS: Register select
     RW: Read/ write selection
     E(Enable): Enable data transfer
     D4: Data pin 4
     D5: Data pin 5
     D6: Data pin 6
     D7: Data pin 7
     A(BKlt+): Anode LED backlight
     K(BKlt-): Cathode LED backlight
   5- References: https://docs.arduino.cc/built-in-examples/digital/Debounce
   https://docs.arduino.cc/learn/electronics/lcd-displays
   https://docs.arduino.cc/built-in-examples/digital/Button
   https://www.arduino.cc/reference/en/libraries/liquidcrystal/
   6- Demo: Demonstration uploaded to VoiceThread
*/

#include <LiquidCrystal.h> // include the LiquidCrystal library

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2; // define the pins used for the LCD
const int player1 = 7;  // the number of the pushbutton pin for player 1
const int player2 = 7;  // the number of the pushbutton pin for player 2
int p1Score = 0; // initialize player 1's score
int p2Score = 0; // initialize player 2's score
LiquidCrystal lcd(rs, en, d4, d5, d6, d7); // initialize the LCD library with the pins defined above
int buttonState1 = 0;  // variable for reading the pushbutton status for player 1
int buttonState2 = 0;  // variable for reading the pushbutton status for player 2
int lastButtonState1 = LOW; // previous state of player 1's button
int lastButtonState2 = LOW; // previous state of player 2's button
int p1Round = 1; // initialize player 1's round
int p2Round = 1; // initialize player 2's round
unsigned long lastDebounceTime1 = 0;  // the last time the output pin for player 1 was toggled
unsigned long lastDebounceTime2 = 0;  // the last time the output pin for player 2 was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers
int titleScreen = 1; // initialize title screen flag

void setup() {
  lcd.begin(16, 2); // initialize the LCD with 16 columns and 2 rows
  lcd.setCursor(0, 0); // set the cursor to the top left corner
  lcd.print("Player One"); // print the player 1 label
  lcd.setCursor(0, 1); // set the cursor to the bottom left corner
  lcd.print("Player Two"); // print the player 2 label
  pinMode(player1, INPUT); // set player 1's button pin as input
  digitalWrite(player1, HIGH); // enable internal pull-up resistor for player 1's button
  digitalWrite(player2, HIGH); // enable internal pull-up resistor for player 2's button
}

void loop() {
  int reading1 = digitalRead(player1); // read the state of player 1's button
  
  int reading2 = digitalRead(player2); // read the state of player 2's button
  
  // check if the title screen is displayed and if either player's button is pressed
  if ((titleScreen == 1) && ((reading1 != lastButtonState1) || (reading2 != lastButtonState2))) {
    lcd.clear(); // clear the LCD screen
    titleScreen++; // move on from the title screen
  }
  
  // debounce and handle button presses for player 1's button
  if ((millis() - lastDebounceTime1) > debounceDelay) { // if debounce time has passed
    if (reading1 != buttonState1) { // if button state has changed
      buttonState1 = reading1; // update button state
      if (buttonState1 == LOW) { // if button is pressed
        if (p1Score > 15) { // if score exceeds max score
          lcd.setCursor(0, 0);
          lcd.print("                "); // clear the score area
          p1Score = 0; // reset score
          p1Round++; // move on to next round
        }
        if ((p1Round == 2) && (p1Score == 15)) { // if player has won two rounds and scores 15
          lcd.setCursor(p1Score, 0);
          p1Score++;
          lcd.print("*"); // display a star on LCD
          delay(5000); // delay for 5 seconds
          p1Round--; // go back to one round
          p2Round = 1;
          titleScreen--; // go back to title screen
          lcd.clear(); // clear LCD screen
          setup(); // reset the game
        } else {
          lcd.setCursor(p1Score, 0);
          p1Score++;
          lcd.print("#"); // display a hashtag on LCD
        }
      }
    }
  }
  lastButtonState1 = reading1; // update last button state

  // debounce and handle button presses for player 2's button
  if (reading2 != lastButtonState2) { // if button state has changed
    lastDebounceTime2 = millis(); // update debounce time
  }
  if ((millis() - lastDebounceTime2) > debounceDelay) { // if debounce time has passed
    if (reading2 != buttonState2) { // if button state has changed
      buttonState2 = reading2; // update button state
      if (buttonState2 == LOW) { // if button is pressed
        if (p2Score > 15) { // if score exceeds max score
          lcd.setCursor(0, 1);
          lcd.print("                "); // clear the score area
          p2Score = 0; // reset score
          p2Round++; // move on to next round
        }
        if ((p2Round == 2) && (p2Score == 15)) { // if player has won two rounds and scores 15
          lcd.setCursor(p2Score, 1);
          p2Score++;
          lcd.print("*"); // display a star on LCD
          delay(500); // delay for 0.5 seconds
          p1Round = 1;
          p2Round--; // go back to one round
          titleScreen--; // go back to title screen
          setup(); // reset the game
        } else {
          lcd.setCursor(p2Score, 1);
          p2Score++;
          lcd.print("8");
        }
      }
    }
  }
  lastButtonState2 = reading2; // save the current button state for player 2 for the next iteration of the loop
  delay(100); // delay to debounce button input and prevent flickering
}
