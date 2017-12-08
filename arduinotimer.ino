/***************************
 * Author: github/savaughn *
 **************************/
 
#include <LiquidCrystal.h>
#include <Time.h>

//Specify LCD pins on board
LiquidCrystal lcd(7,8,9,10,11,12);

int state = LOW;      //Default LOW
int inputPin = 2;     //Input location on board
int lastState = LOW;  //Default HIGH
int holdCount = 0;
int reset = 1;

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
  lcd.setCursor(6,1);
  if (state == 0) 
    lcd.print("LOW"); 
  else
    lcd.print("HIGH");
}

void loop(){
   while (reset == 0){
      while (digitalRead(inputPin) == HIGH){
        //do nothing
      }
      reset = 1;         
   }
  
    while (state == LOW && reset == 1){
       while (digitalRead(inputPin) == HIGH){
        lcd.setCursor(0,0);
        lcd.clear();
         lcd.print(holdCount++);
       }
        if(holdCount < 1001){
          holdCount = 0;        
      } if (holdCount > 999){
        state = HIGH;         
      }
    }
    
    //Keeps state HIGH until button is pushed
    if (lastState == HIGH && digitalRead(inputPin) == LOW ){
      state = HIGH;
      }
    
    //Returns to LOW when button is pushed when already in HIGH state
    else if (lastState == HIGH && state == HIGH ){
      state = LOW;
      reset = 0;
    }
    
    //updates LCD when state change occurs
    if (state != lastState){    
      print(state);        
    } 
    
    //track last state
    lastState = state;
    holdCount = 0;

  
}

/****************************************************************
TODO: 1. Add mandatory hold period in transition from LOW to HIGH
      2. Add LED HIGH/LOW indicators
      3. Implement Timer
      4. Display previous time
      5. Implement scramble page  
*******************************************************************/
