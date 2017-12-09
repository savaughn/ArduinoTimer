/***************************
 * Author: github/savaughn *
 **************************/

#include <LiquidCrystal.h>
#include <Time.h>

//Specify LCD pins on board
LiquidCrystal lcd(7,8,9,10,11,12);

byte block[8] = {
  B00000,
  B01110,
  B11111,
  B11111,
  B11111,
  B01110,
  B00000,
};

int state = LOW;      //Default LOW
int inputPin = 2;     //Input location on board
int cancelPin = 3;
int bestPin = 4;
int lastState = LOW;  //Default HIGH
int reset = 1;
int debounce = 4;     //Delay from pushbutton
unsigned long time;
unsigned long timeStart;
unsigned long timeStop = 0;
unsigned long timeBest;
unsigned long currentTime;
unsigned long timePrev = 0;

void setup(){
  lcd.createChar(0, block);
  lcd.begin(16,2);               //Initialize LCD
  timeInit(0);
  timeInit(1);
}

void clearLine(int line){            //Because clear() can't work on just one line???
  lcd.setCursor(0, line);
  lcd.print("                ");
}

void clearTime(int line){
  lcd.setCursor(6,line);
  lcd.print("          ");
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

void timeInit(int line){
  lcd.setCursor(4,line);
  lcd.print("00:00:000   ");
  lcd.setCursor(4,line);
  lcd.print("00:00:000   "); 
}

void printTimer(unsigned long printTime, int line){

  unsigned long ms = printTime%1000;
  int s = (printTime/1000)%60;
  int m = (printTime/60000)%60;

  lcd.setCursor(10,line);
  lcd.print( ms );
  if (s < 10){
    lcd.setCursor(7,line);
    lcd.print(0);
    lcd.setCursor(8,line);
  }
  else
    lcd.setCursor(7,line);
  lcd.print( s );

  if(m < 10){
    lcd.setCursor(4,line);
    lcd.print(0);
    lcd.setCursor(5,line);       
  }
  else
    lcd.setCursor(4,line);
  lcd.print(m);

}

void cancelLast(){
  lcd.setCursor(15,2);
  lcd.write(byte(0)); 
  timeStop = 0;
  timeInit(0);
}

void sessionBest(unsigned long timeStop,  unsigned long timePrev, unsigned long timeBest){
  lcd.clear();
  timeInit(1);
  while(digitalRead(bestPin) == HIGH){          
    lcd.setCursor(2,0);
    lcd.print("Session Best");         
    lcd.setCursor(6,1);
    printTimer(timeBest, 1);
  }
  lcd.clear();
  timeInit(0);
  timeInit(1);
  printTimer(timeStop,0);
  printTimer(timePrev, 1); 
}


  
void loop(){
  //Waiting for user to start timer
  while (state == LOW && reset == 1){

    //check for cancel button
    if (digitalRead(cancelPin) == HIGH){
      cancelLast();
    }
    
    //check for session best button
    if (digitalRead(bestPin) == HIGH){
      sessionBest(timeStop, timePrev, timeBest);
    }    
    
    
    
    time = millis();
    while (digitalRead(inputPin) == HIGH){         
      drawBlock(1);  
      
      //save last time
      if (timePrev != timeStop)
        timePrev = timeStop;
      
      //moves previous time to bottom row
      timeInit(0);
      printTimer(timePrev, 1);
      
      //Draws top (go) block
      if (millis()- time > 550) {
        drawBlock(0);
      }
    }

    if(millis() - time < 550){    //Button wasn't held long enough
      time = millis();            //reset state change timer
      clearBlocks();
    } 
    else {                        //Button was held long enough
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

  //Timer is running  
  while(state == HIGH){      
    currentTime = (millis()-timeStart); 
    printTimer(currentTime, 0);     

    //Keeps state HIGH until button is pushed
    if (lastState == HIGH && digitalRead(inputPin) == LOW ){
      state = HIGH;
    }

    //Returns to LOW when button is pushed when already in HIGH state
    else if (lastState == HIGH && digitalRead(inputPin) == HIGH ){       
      timeStop = currentTime;     
      if(timeBest == 0)
        timeBest = timeStop;
      else if(timeStop < timeBest){
        timeBest = timeStop;
        lcd.setCursor(15,0);
        lcd.write(byte(0));
      }
        
      state = LOW;
      reset = 0;        //prevents state change timer from starting from HIGH to LOW
    }

    //track last state
    lastState = state;
  }
}

/****************************************************************
 * TODO: 5. Implement scramble page  
 *******************************************************************/

