#include <Servo.h>
#include <Wire.h>
 
long int printCount = 0;
int printNr = 4;
int printTime = 100000;

int resPin1 = A0;
int resPin2 = A1;
int resPin3 = A2;
int resPin4 = A3;
int res1;
int res2;
int res3;
int res4;

int xpin = A6;
int Ypin = A7;

int absDiffX; 
int absDiffY; 

int xKord;
int xpos;
int ypos; 
int Y;
int X;
int defaultXPos = 121;
int defaultYPos = 87;
int xMax = 155;
int yMax = 180;
int xMin = 0;
int yMin = 0;
int sens1 = 1;
int sens2 = 2;

int state;
int count;
int leftAvr;
int rightAvr;
int upAvr;
int downAvr;
int diff = 8;

int buttonPin = 13;
int bState;
int Start;

int servo_x_pin = 6;
int servo_y_pin = 5;

//byte ledState;

boolean mFlag = true;
boolean aFlag = true; 
boolean bFlag = true;
boolean sFlag = true;
boolean tFlag = true;
boolean sunPrint = true;
boolean startMode = true;
boolean xStop = true;
boolean yStop = true;

Servo servoX;
Servo servoY;

void sendStandby();
void sendAuto();
void sendManual();

void setup() { 
  
  servoX.attach(servo_x_pin);
  servoY.attach(servo_y_pin);

  digitalWrite(buttonPin,HIGH);
  
  pinMode(resPin1,INPUT);       
  pinMode(resPin2,INPUT);
  pinMode(resPin3,INPUT);
  pinMode(resPin4,INPUT); 
  pinMode(buttonPin,INPUT);
  pinMode(xpin,INPUT);
  pinMode(Ypin,INPUT);
 
  X = defaultXPos;
  Y = defaultYPos;
  
  servoX.write(X);
  servoY.write(Y);

  Serial.begin(9600);   
  Wire.begin();
}

void loop() {
  switch(mode()){
    case 0: 
      if(mFlag){      
        sendManual();
        mFlag = false;
        sFlag = true;  
      }
      manTrack();
    break;

    case 1:
      if(aFlag){    
        sendAuto();
        aFlag = false; 
      }    
      autoTrack();
        
    break;

    case 2:  
      if(sFlag){
        sendStandby();
        sFlag = false; 
        printResVal(); 
        }
    
    break;
    default :  
      Serial.println("Joystick error!");
    
  }
}

void sendStandby(){
  Wire.beginTransmission(88); 
  Wire.write('s');
  Wire.endTransmission();
}

void sendAuto(){
  Wire.beginTransmission(88); 
  Wire.write('a');
  Wire.endTransmission();
}

void sendManual(){
  Wire.beginTransmission(88); 
  Wire.write('m');
  Wire.endTransmission();
}

void manTrack(){
  while(mode() == 0){
    yposFunction(analogRead(Ypin));
    xposFunction(analogRead(xpin)); 
    resVal();
    printResVal();  
  }
  mFlag = true;
}

void autoTrack(){ 
  while(mode() == 1){
  newPos();
  printResVal();   
  } 
  aFlag = true;
}

int mode(){
  //bState = buttonState();
  bState = digitalRead(buttonPin);
  if (bState == HIGH && bFlag == true) {
    count++;
    if (count >= 3) {
      count = 0;
    }
    bFlag = false;
    }
    else if (bState == LOW && bFlag == false) {
      bFlag = true;
    }
    if(startMode){  
      startMode = false;
      count = 2;
      }
      else{
    return count;
  }
}

int buttonState(){
  int i;
  byte state;
  state = digitalRead(buttonPin);
  state = state^1;
}

void change(){
  resVal();
  leftAvr = (res2 + res3)/2;
  rightAvr = (res1 + res4)/2;
  upAvr = (res3 + res4)/2;
  downAvr = (res2 + res3)/2;
  absDiffX = abs(leftAvr-rightAvr);
  absDiffY = abs(downAvr-upAvr);
}

void newPos(){
  change();
  if(absDiffX>diff){
    if(leftAvr < rightAvr){
      ++X;
      if(X > xMax){
        X = xMax;
      }
    }
    else if(leftAvr > rightAvr){
      --X;
      if(X < xMin){
        X = xMin;
      }
    }
    servoX.write(X);
  }
  
  if(absDiffY>diff){
    if(downAvr < upAvr){
      ++Y;
      if(Y > yMax){
        Y = yMax; 
      }
    }
    else if(downAvr > upAvr){
      --Y;
      if(Y < yMin){
        Y = yMin;
      }
    }
    servoY.write(Y);
  } 
}  

void xposFunction(int xpos){
  if(xpos < 490){
    servoX.write(X+=sens1);
  } 
  if(xpos < 10){  
    servoX.write(X+=sens2); 
  } 
  if(xpos > 525){
    servoX.write(X-=sens1);
  } 
  if(xpos > 990){  
    servoX.write(X-=sens2);
  }  
  if(X >= 155){
    X = 155;
  } 
  if(X <= 0){
    X = 0;
  } 
}

void yposFunction(int ypos){
  if(ypos < 490){
    servoY.write(Y+=sens1);
   } 
   if(ypos < 10){  
    servoY.write(Y+=sens2); 
   } 
   if(ypos > 525){
    servoY.write(Y-=sens1);
   } 
   if(ypos > 996){  
    servoY.write(Y-=sens2);
   }  
   if(Y >= 180){
    Y = 180;
   } 
   if(Y <= 0){
    Y = 0;       
   } 
}

void printResVal(){
  resVal();
  change(); 
  Serial.print("1#(");
  Serial.print(res1);
  Serial.print(") 2#(");
  Serial.print(res2);
  Serial.print(") 3#(");
  Serial.print(res3);
  Serial.print(") 4#(");
  Serial.print(res4);
  Serial.print(") // Xabs |");
  Serial.print(absDiffX);
  Serial.print("| // Yabs |");
  Serial.print(absDiffY);
  Serial.print("| X: ");
  Serial.print(X);
  Serial.print("| Y: ");
  Serial.print(Y);
  Serial.print(" |#| ");
  if(mode()==0){
    Serial.println("    #Manual mode#");
  }
  if(mode()==1){
    Serial.println("    #Auto mode#");
  }
  if(mode()==2){
    Serial.println("    #Standby  mode#");
  }
}

void resVal(){
  res1 = map(analogRead(resPin1),0, 650, 0 ,180);
  res2 = map(analogRead(resPin2),0, 650, 0 ,180);
  res3 = map(analogRead(resPin3),0, 650, 0 ,180);
  res4 = map(analogRead(resPin4),0, 650, 0 ,180);
} 
