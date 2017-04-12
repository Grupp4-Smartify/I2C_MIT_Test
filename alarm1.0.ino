#include <Wire.h>

int buttonPin = 2;
int alarmPin[3] = {5,6,7};

boolean bFlag = true;
boolean alarm = false;
boolean buzzer = false;

volatile byte  Stop1 = 0;
volatile byte  Stop2 = 0;

const byte slaveId = 2;

void setup(){
  Wire.begin(slaveId); 
  Wire.onReceive(receiveEvent);
  attachInterrupt(digitalPinToInterrupt(buttonPin), stopAlarm, CHANGE);

  pinMode(buttonPin,INPUT); 
}

void loop(){
  if(buttonMode() || alarm)-{
    Stop1 = 1;  
  }
  if(Stop1 && Stop2){
      for(int i=0;i<=2;i++){
        if(digitalRead(alarmPin[i])==LOW){
          buzzer = true;
        }     
        if(buzzer){
          alarmSound();
          }
        }
    }
    if(!buttonMode()){
      Stop2 = 1;
    }
}

void stopAlarm(){
  Stop1 = 0;
  Stop2 = 0;
}

void receiveEvent(int howMany){
  char inChar;
  while(Wire.available() > 0){
    inChar = Wire.read();  
    
    if (inChar == 'H'){
      alarm = true;
      Stop1 = 1;
      Stop2 = 1;
    }
    else if (inChar == 'L'){
      alarm = false;
      Stop1 = 0;
      Stop2 = 0;
    }
  }
}

void alarmSound(){
      Serial.println("Alarm!!!");
      tone(buzzer, 4000,100);
      noTone;
      delay(500);
      tone(buzzer, 4000,100);
      noTone;
      delay(500);  
}

boolean buttonMode(){
  int mode;
  int bState = digitalRead(buttonPin);
  if (bState == HIGH && bFlag == true){ 
    bFlag = false;
    mode++;
    if(mode >= 1) {
      mode = 0;
    }  
    else if (bState == LOW && bFlag == false){
    bFlag = true;
    }
    switch(mode){
      case 0: return false;
      case 1: return true;
    }
  }
}
