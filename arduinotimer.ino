/***************************
 * Author: github/savaughn *
 **************************/
 
#include <LiquidCrystal.h>
#include <Time.h>

//Specify LCD pins on board
LiquidCrystal lcd(7,8,9,10,11,12);

int state = LOW;      //Default LOW
int inputPin = 2;     //Input location on board
int lastState = LOW;  //Default LOW

void setup(){
  
  lcd.begin(16,2);               //Initialize LCD
  lcd.setCursor(0,0);            //Initialize Cursor
  lcd.print("READY");  
  delay(1000);
  lcd.clear();
  print(digitalRead(inputPin));  //Print initial status
}

//Print method defines states as HIGH or LOW
void print(int state){
  lcd.clear();
  if (state == 0) 
    lcd.print("LOW"); 
  else
    lcd.print("HIGH");
}

void loop(){
  //get current state of button (default LOW)  
  state = digitalRead(inputPin);
  
  //make sure button was released
  int buttonRelease = digitalRead(inputPin);
  while(buttonRelease == state ){
    buttonRelease = digitalRead(inputPin);
  } 
  
  //Keeps state HIGH until button is pushed
  if (lastState == HIGH && state == LOW ){
    state = HIGH;
  }
  
  //Returns to LOW when button is pushed when already in HIGH state
  else if (lastState == HIGH && state == HIGH ){
    state = LOW;    
  }
  
  //updates LCD when state change occurs
  if (state != lastState){      
    print(state);         
  } 
  
  //track last state
  lastState = state;
}

/****************************************************************
TODO: 1. Add mandatory hold period in transition from LOW to HIGH
      2. Add LED HIGH/LOW indicators
      3. Implement Timer
      4. Display previous time
      5. Implement scramble page  
*******************************************************************/
