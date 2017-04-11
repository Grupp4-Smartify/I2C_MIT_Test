


int bState;
int buttonPin = 2;
int aPin[3]= {5,6,7};
int count;
int buzzer = 4;

boolean bFlag;
boolean aOn;
boolean aOff;
boolean alarm;
boolean stopFlag;
const byte slaveId = 1;



void setup() {

  delay(3000);
  Serial.begin(9600);
  pinMode(buttonPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(buttonPin), stopAlarm, CHANGE);
  pinMode(aPin[0], INPUT);
  pinMode(aPin[1], INPUT);
  pinMode(aPin[2], INPUT);
  pinMode(aPin[3], INPUT);
  bFlag = true;
  aOn = true;
  aOff = true;
  alarm = false;
  stopFlag = true;
  Wire.onRequest(requestEvent);
  Wire.onReceive(receiveEvent);

}



void loop() {
  
}

void stopAlarm(){
  //Serial.println("Interrupt");
  alarm = false;
}

boolean alarmState(){
 for(int i;i<3;i++){
  if(digitalRead(aPin[i]) == LOW){
    alarm = true;
  }
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

void requestEvent(){
  
  if(aOff){
    aOff = false;
    Wire.println("Alarm: Off");
    aOn = true;
  }

  while(bMode() && stopFlag){

    if(aOn){
      aOn = false;
      Wire.println("Alarm: On");
      aOff = true;
    }

    alarmState();
    
    if(alarm){
      Wire.println("Alarm!!!");
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

void receiveEvent(){
    
  }

