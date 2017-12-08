/***************************
 * Author: github/savaughn *
 **************************/
 
#include <LiquidCrystal.h>
#include <Time.h>

//Specify LCD pins on board
LiquidCrystal lcd(7,8,9,10,11,12);

byte block[8] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
};

int state = LOW;      //Default LOW
int inputPin = 2;     //Input location on board
int lastState = LOW;  //Default HIGH
int holdCount = 0;
int reset = 1;
int timer = 0;

void setup(){
  lcd.createChar(0, block);
  lcd.begin(16,2);               //Initialize LCD
  lcd.setCursor(0,0);            //Initialize Cursor
  lcd.print("READY");  
  delay(1000);
  lcd.clear();
  print(digitalRead(inputPin));  //Print initial status
}

void clearLine(int line){            //Because clear() can't work on just one line???
  lcd.setCursor(0, line);
  lcd.print("                ");
}

void clearTime(int line){
  lcd.setCursor(6,line);
  lcd.print("        ");
}

void clearBlock(){
 lcd.setCursor(0,1);
   lcd.print(" "); 
}

void loop(){
   //Initial state LOW mandatory hold HIGH 1 second before state change to HIGH
    while (state == LOW && reset == 1){
       while (digitalRead(inputPin) == HIGH){
           lcd.setCursor(0,1);
           lcd.write(byte(0));
          holdCount++;    //TODO: LED instead of count print
          if(holdCount > 999){
          lcd.setCursor(0,0);
          lcd.write(byte(0));
        }
       }
        if(holdCount < 1000){      //Button wasn't held long enough
          holdCount = 0;           //reset state change timer
      } else {      //Button was held long enough
        state = HIGH;              //Change state
        holdCount = 0;
      }
    }
    
    //Busy wait if from HIGH to LOW the button has not been let go
    while (state == LOW && reset == 0){
      while (digitalRead(inputPin) == HIGH){
        //do nothing
      }
      reset = 1;
      clearTime(1);
      lcd.setCursor(6,1);
      lcd.print(timer);
      clearLine(0); 
      clearBlock();
      timer = 0;     
   }
  
    while(state == HIGH){
      
      
      lcd.setCursor(6,0);
      lcd.print(timer++);          //TODO: convert to TIME
    
      //Keeps state HIGH until button is pushed
      if (lastState == HIGH && digitalRead(inputPin) == LOW ){
        state = HIGH;
        }
      
      //Returns to LOW when button is pushed when already in HIGH state
      else if (lastState == HIGH && digitalRead(inputPin) == HIGH ){
        state = LOW;
        reset = 0;        //prevents state change timer from starting from HIGH to LOW
      }
      
      //track last state
      lastState = state;
  }
}

/****************************************************************
TODO: 5. Implement scramble page  
*******************************************************************/
