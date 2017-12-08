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
int reset = 1;
int debounce = 1;     //Delay from pushbutton
unsigned long time;
unsigned long timeStart;
unsigned long timeStop = 0;

void setup(){
  lcd.createChar(0, block);
  lcd.begin(16,2);               //Initialize LCD
  timeInit();
}

void clearLine(int line){            //Because clear() can't work on just one line???
  lcd.setCursor(0, line);
  lcd.print("                ");
}

void clearTime(int line){
  lcd.setCursor(6,line);
  lcd.print("        ");
}

void clearBlocks(){
  lcd.setCursor(0,0);
   lcd.print(" "); 
 lcd.setCursor(0,1);
   lcd.print(" "); 
}

void drawBlock(int line){
  lcd.setCursor(0,line);
  lcd.write(byte(0));
}

void timeInit(){
   lcd.setCursor(4,0);
  lcd.print("00:00:00"); 
}

void printTimer(unsigned long currentTime){
 
   unsigned long ms = currentTime%100;
      int s = (currentTime/1000)%60;
      int m = (currentTime/60000)%60;
      
     lcd.setCursor(10,0);
     lcd.print( ms );
     if (s < 10){
       lcd.setCursor(7,0);
       lcd.print(0);
       lcd.setCursor(8,0);
     }
     else
       lcd.setCursor(7,0);
     lcd.print( s );
     
     if(m < 10){
       lcd.setCursor(4,0);
       lcd.print(0);
       lcd.setCursor(5,0);
       
     }
     else
       lcd.setCursor(4,0);
     lcd.print(m);
  
}

void loop(){
   //Initial state LOW mandatory hold HIGH 1 second before state change to HIGH
    while (state == LOW && reset == 1){
      time = millis();
       while (digitalRead(inputPin) == HIGH){
           drawBlock(1);  
           if(timeStop != 0){
             lcd.setCursor(4,1);
             lcd.print(timeStop);
             timeInit();
           }
                 
         if (millis()- time > 550) {
          drawBlock(0);
        }
       }
        if(millis() - time < 550){    //Button wasn't held long enough
          time = millis();            //reset state change timer
          clearBlocks();
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
      clearBlocks();         
   }
  
    while(state == HIGH){      
      
      unsigned long currentTime = (millis()-timeStart);      
      printTimer(currentTime);
     
    
      //Keeps state HIGH until button is pushed
      if (lastState == HIGH && digitalRead(inputPin) == LOW ){
        state = HIGH;
        }
      
      //Returns to LOW when button is pushed when already in HIGH state
      else if (lastState == HIGH && digitalRead(inputPin) == HIGH ){
        
        timeStop = millis()-timeStart - debounce;
        state = LOW;
        reset = 0;        //prevents state change timer from starting from HIGH to LOW
        lcd.setCursor(6,0);
        lcd.print(timeStop);
      }
      
      //track last state
      lastState = state;
  }
}

/****************************************************************
TODO: 5. Implement scramble page  
*******************************************************************/
