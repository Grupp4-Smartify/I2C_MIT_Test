#include <Wire.h>

byte bState;
int buttonPin = 2;
int aPin[3]={6,7,8};
int pirSensor = 3;
int count;
int countL;
int buzzer = 4;
int ledPin = 9;

boolean pirOn;
boolean buttonOff;
boolean on;
boolean bFlag;
boolean aOn;
boolean aOff;
boolean alarm;
boolean stopFlag;
const byte slaveId = 1;
byte state = 0;



void setup() {

  delay(3000);
  Serial.begin(9600);
  pinMode(buttonPin, INPUT);
 // attachInterrupt(digitalPinToInterrupt(buttonPin), stopAlarm, CHANGE);
  pinMode(aPin[0], INPUT);
  pinMode(aPin[1], INPUT);
  pinMode(aPin[2], INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(pirSensor, INPUT);
  bFlag = true;
  aOn = true;
  aOff = true;
  alarm = false;
  stopFlag = true;
  buttonOff = false;
  Wire.onRequest(requestEvent);
  Wire.onReceive(receiveEvent);
  Wire.begin(slaveId);
}



void loop() {


  if(aOff){
    aOff = false;
    Serial.println("Alarm: Off");
    aOn = true;
  }

  while(on && stopFlag){
   alarmLoop();
  }
 }

/*
void stopAlarm(){
  //Serial.println("Interrupt");
  alarm = false;
  on = false;
  buttonOff = true;
  digitalWrite(ledPin, LOW);
}
*/


void hallState(){
  for(int i=0;i<3;i++){
  if(digitalRead(aPin[i]) == HIGH){
    alarm = true;
  }
 }
}

void pirState (){
  
  if (digitalRead(pirSensor) == HIGH) {
      alarm = true;
  }
}





/*int bMode() {
  
  bState = digitalRead(buttonPin);
  bState = bState^1;
  if (bState == 1 && bFlag == true) {
    count++;
    if (count >= 2) {
      count = 0;
    }
    bFlag = false;
  } 
  else if (bState == 0 && bFlag == false) {
    bFlag = true;
  }
  if(buttonOff == true){
    buttonOff = false;
    return 0; 
  }else{
     return count;
  }
}
*/


void alarmLoop(){

     
    if(aOn){
      aOn = false;
      Serial.println("Alarm: On");
      aOff = true;
    }
    
     countL++;
     if(countL == 20000){
      countL = 0;
      state = state^1;
      digitalWrite(ledPin, state);
      }
     
    hallState();
    if(pirOn == true){
    pirState();
    }
    
    if(alarm){
      Serial.println("Alarm!!!");
      digitalWrite(ledPin, HIGH);
      tone(buzzer, 4000,100);
      noTone;
      delay(200);
      tone(buzzer, 4000,100);
      noTone;
      delay(200);
     
  stopFlag = true;
    }
  }

void requestEvent(){
  if(alarm){
    Wire.print('1');
    }else{
    Wire.print('0');
    }
  }
 

void receiveEvent(int howMany){
    char inChar;
    while(Wire.available() > 0){
      inChar = Wire.read();
      if (inChar == 'H'){
        on = true;
      //  buttonOff = false;
        }
       if(inChar == 'L'){
        on = false;
        alarm = false;
       //  buttonOff = true;
         pirOn = false;
        digitalWrite(ledPin, LOW);
        }
        if(inChar == 'X'){
         pirOn = true;
        }
        if(inChar == 'Y'){
         pirOn = false;
        }
      
   }
}
      
      
      

