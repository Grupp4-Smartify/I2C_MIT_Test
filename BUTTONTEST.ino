#include <Wire.h>

int buttonPin = 12;     // the number of the pushbutton pin
int alarmPin = 11; 
 int buttonState;
 boolean buttonSet;
 int alarmState;
 boolean alarmSet;

void setup() {
 int buttonState = 0;
 boolean buttonSet = false;
 int alarmState = 0;
 boolean alarmSet = false;

  // initialize the pushbutton pin as an input:
  Serial.begin(9600);
  pinMode(buttonPin, INPUT);
  pinMode(alarmPin, INPUT);
  //Wire.begin(slaveId); 
  //Wire.onReceive(receiveEvent);
 // Wire.onRequest(requestEvent);
}

void loop() {
    alarmRead();
  if (alarmSet){
    buttonRead();
  }
}

 void buttonRead(){
 
 buttonState = digitalRead(buttonPin);//blue


  if (buttonState == LOW) {
   
    Serial.println("ALARM!ALARM!");
  } else {
    
    Serial.println("Säkert");
  }
 }
 
 void alarmRead(){
 
 alarmState = digitalRead(alarmPin);//red
 

  if (alarmState == LOW) {
   if (alarmSet == true){
    alarmSet = false;
   }else if (alarmSet == false){
    alarmSet = true;
   }
  } 
 }









 

