#include <LiquidCrystal.h>
#include <Servo.h>
#include <Wire.h>

LiquidCrystal lcd(7, 8, 9, 10, 11, 12); // RS,E,D4,D5,D6,D7

byte sun1[8] = {
  B00010,
  B00001,  
  B00000,  
  B10010,  
  B01001,  
  B00000,  
  B10010,  
  B01000 
};

byte sun2[8] = { 
  B01001,
  B00100,
  B10000,
  B01011,
  B00111,
  B01111,
  B11111,
  B11111  
};

byte sun4[8] = {
  B11111,
  B11111,
  B01111,
  B00111,
  B01011,
  B10000,
  B00100,
  B01001  
};

byte sun3[8] = {
  B01000,
  B10010,  
  B00000,  
  B01001,  
  B10010,  
  B00000,  
  B00001,  
  B00010  
};

byte tracking1[8] = {
  B00000,
  B00000,
  B00001,
  B00010,
  B00100,
  B01100,
  B10100,
  B00100
};

byte tracking2[8] = {
  B00000,
  B00000,
  B00000,
  B11111,
  B00100,
  B00100,
  B00100,
  B00100,
};

byte tracking3[8] = {
  B00000,
  B10000,
  B01000,
  B00100,
  B00110,
  B00101,
  B00100,
  B00100
};

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
int diff = 4;

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

void setup() {
 
  lcd.createChar(0, sun1);
  lcd.createChar(1, sun2);
  lcd.createChar(2, sun3);
  lcd.createChar(3, sun4);
  lcd.createChar(4, tracking1);
  lcd.createChar(5, tracking2);
  lcd.createChar(6, tracking3); 
  
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
  
  lcd.begin(16,2);
  lcd.clear();
  lcd.print("Ready");
  for(int i=3;i>=0;i--){
    delay(1000);
    lcd.setCursor(6,0);
    lcd.print("");
    lcd.setCursor(6,0);
    lcd.print(i);
  }
  printSun();
  Serial.begin(9600); 
  Wire.begin();
}

void loop() {
  switch(mode()){
    case 0: 
      if(mFlag){  
        mFlag = false;
        sFlag = true; 
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Manual mode");
        lcdPrintPos(); 
        printSun(); 
      }
      manTrack();
    break;

    case 1:
      if(aFlag){    
        aFlag = false;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Tracking mode");   
        lcdPrintPos(); 
        printSun();
      }    
      autoTrack();
        
    break;

    case 2:  
      if(sFlag){
        sFlag = false;
        lcd.clear(); 
        lcd.setCursor(0,0);
        lcd.print("Standby mode");
        lcd.setCursor(13,1);
        lcd.write(byte(6));
        printResVal();
        lcdPrintPos();
        lcdPrintNewPos();
        printSun(); 
        }
    
    break;
    default : 
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("-1");
      Serial.println("Joystick error!");
    
  }
}
void sendPos(){
  Wire.beginTransmission(1); 
  Wire.write(88);
  Wire.endTransmission();
} 

void manTrack(){
  while(mode() == 0){
    yposFunction(analogRead(Ypin));
    xposFunction(analogRead(xpin)); 
    resVal();
    printResVal();
    lcdPrintNewPos(); 
    printTracking();
  }
  mFlag = true;
}

void autoTrack(){
  while(mode() == 1){
  newPos();
  printResVal();
  lcdPrintNewPos(); 
  printTracking();
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

void printTracking(){ 
  if(printCount == 10){  
    printCount = 0;
    lcd.setCursor(13,1);
    if(printNr == 7){
      printNr = 4; 
      lcd.print("");
      lcd.write(byte(5));
    }else{
      lcd.print("");
      lcd.write(byte(printNr++)); 
    }
  }      
  printCount++; 
}

void lcdPrintNewPos(){
  lcd.setCursor(2,1);
  lcd.print("   ");
  lcd.setCursor(2,1);
  lcd.print(X);
  lcd.setCursor(8,1);
  lcd.print("   ");
  lcd.setCursor(8,1); 
  lcd.print(Y);
}

void lcdPrintPos(){
  lcd.setCursor(0,1);
  lcd.print("X:");
  lcd.setCursor(5,1);
  lcd.print("|");
  lcd.setCursor(6,1);
  lcd.print("Y:");
}

void printSun(){
  lcd.setCursor(14,0);
  lcd.write(byte(0));
  lcd.setCursor(15,0);
  lcd.write(byte(1));
  lcd.setCursor(14,1);
  lcd.write(byte(2));
  lcd.setCursor(15,1);
  lcd.write(byte(3));
}

void resVal(){
  res1 = map(analogRead(resPin1),0, 650, 0 ,180);
  res2 = map(analogRead(resPin2),0, 650, 0 ,180);
  res3 = map(analogRead(resPin3),0, 650, 0 ,180);
  res4 = map(analogRead(resPin4),0, 650, 0 ,180);
}  
