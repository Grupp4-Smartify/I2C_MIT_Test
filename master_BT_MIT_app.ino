int ledPin = 13;
int potPin = A5;

char c;

#include <Wire.h>

String readString;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Wire.begin();
  pinMode(LED_BUILTIN,OUTPUT); 
}

void loop() {
  
  while (Serial.available()){
    delay(3);
    c = Serial.read();
    readString += c; 
  }
  if (readString.length()>0){
     //Serial.println(readString);
  if (readString == "on"){
     digitalWrite(LED_BUILTIN,HIGH);
     on();
     }
  if (readString == "off"){
     digitalWrite(LED_BUILTIN,LOW);
     off();
     }
  if(readString == "s1"){
     readTemp();
    }
  if(readString.startsWith("D",0)){
     //Serial.println("-1");
     neoPix();
    }    
    readString="";
  }  
}

void neoPix(){
  Serial.println("-1");
  delay(2500);
  Wire.beginTransmission(2);
  //Serial.println("Neo command: ");
  Wire.write('H');
  Wire.print(readString);
  Serial.print("0q");
  Serial.println(readString);
  Wire.endTransmission();   
}

void readTemp(){
    Wire.requestFrom(1, 8);    // request 6 bytes from slave device #8
      while (Wire.available()) {  
      char c = Wire.read(); 
      //Serial.print(c);          
  }
}

void on(){
  Wire.beginTransmission(2);
  //Serial.println("H to slave 1");
  Wire.write('H');
  Wire.endTransmission();    
}

void off(){
  Wire.beginTransmission(2);
  //Serial.println("L to slave 1");
  Wire.write('L');
  Wire.endTransmission();
}

