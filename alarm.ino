int bState;
int buttonPin = 2;
int aPin = 3;
int count;
int buzzer = 4;

boolean bFlag;
boolean aOn;
boolean aOff;
boolean alarm;
boolean stopFlag;

void setup() {
  delay(3000);
  Serial.begin(9600);
  pinMode(buttonPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(buttonPin), stopAlarm, CHANGE);
  pinMode(aPin, INPUT);

  bFlag = true;
  aOn = true;
  aOff = true;
  alarm = false;
  stopFlag = true;
  
}

void loop() {
  if(aOff){
    aOff = false;
    Serial.println("Alarm: Off");
    aOn = true;
  }
  while(bMode() && stopFlag){
    if(aOn){
      aOn = false;
      Serial.println("Alarm: On");
      aOff = true;
    }
    alarmState();
    if(alarm){
      Serial.println("Alarm!!!");
      tone(buzzer, 4000,100);
      noTone;
      delay(500);
      tone(buzzer, 4000,100);
      noTone;
      delay(500);
      
  stopFlag = true;
    }
  }

}

void stopAlarm(){
  //Serial.println("Interrupt");
  alarm = false;
}

boolean alarmState(){
  if(digitalRead(aPin) == HIGH){
    alarm = true;
  }
}

int bMode() {
  bState = digitalRead(buttonPin);
  if (bState == HIGH && bFlag == true) {
    count++;
    if (count >= 2) {
      count = 0;
    }
    bFlag = false;
  } 
  else if (bState == LOW && bFlag == false) {
    bFlag = true;
  }
  return count;
}
