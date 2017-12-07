#include <LiquidCrystal.h>
#include <Time.h>


LiquidCrystal lcd(7,8,9,10,11,12);

int state = LOW;
int inputPin = 2;
int lastState = LOW;
int mutex = 0;

void setup(){
  
  lcd.begin(16,2);
  lcd.setCursor(0,0);
  lcd.print("READY");  
  delay(1000);
  lcd.clear();
  print(digitalRead(inputPin));
}
void print(int state){
  lcd.clear();
  if (state == 0) 
    lcd.print("LOW"); 
  else
    lcd.print("HIGH");
}

void loop(){
     
     state = digitalRead(inputPin);
    
     int buttonRelease = digitalRead(inputPin);
     while(buttonRelease == state ){
        buttonRelease = digitalRead(inputPin);
      } 
    
    if (lastState == HIGH && state == LOW ){
      state = HIGH;
      mutex = 1;
    }
    else if (lastState == HIGH && state == HIGH ){
      state = LOW; 
      mutex = 0;      
  }
    
    
    if (state != lastState){      
      print(state); 
     
      delay(00);       
    } 
      
      lastState = state;
      
      
        
  }

  

