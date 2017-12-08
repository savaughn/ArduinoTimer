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
unsigned long time;
unsigned long timeStart;
unsigned long timeStop;

void setup(){
  lcd.createChar(0, block);
  lcd.begin(16,2);               //Initialize LCD
  initTime();                    //Print initial status
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

void initTime(){
  clearLine(0);
 lcd.setCursor(4,0);
  lcd.print("00:00:00"); 
}

void loop(){
   //Initial state LOW mandatory hold HIGH 1 second before state change to HIGH
    while (state == LOW && reset == 1){
      time = millis();
       while (digitalRead(inputPin) == HIGH){
           lcd.setCursor(0,1);
           lcd.write(byte(0));
           
          if (millis()- time > 550) {
          lcd.setCursor(0,0);
          lcd.write(byte(0));
        }
       }
        if(millis() - time < 550){    //Button wasn't held long enough
          time = millis();            //reset state change timer
      } else {                        //Button was held long enough
        state = HIGH;                 //Change state
        timeStart = millis();
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
      lcd.print(timeStop);       
      initTime(); 
      clearBlock();      //clears bottom block
         
   }
  
    while(state == HIGH){
      
      
      lcd.setCursor(6,0);
      lcd.print(millis() - timeStart);          
    
      //Keeps state HIGH until button is pushed
      if (lastState == HIGH && digitalRead(inputPin) == LOW ){
        state = HIGH;
        }
      
      //Returns to LOW when button is pushed when already in HIGH state
      else if (lastState == HIGH && digitalRead(inputPin) == HIGH ){
        state = LOW;
        reset = 0;        //prevents state change timer from starting from HIGH to LOW
        timeStop = millis()-timeStart;
      }
      
      //track last state
      lastState = state;
  }
}

/****************************************************************
TODO: 5. Implement scramble page  
*******************************************************************/
