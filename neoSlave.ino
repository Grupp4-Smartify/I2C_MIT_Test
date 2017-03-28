#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

int ledPin = 13;

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN            10
// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      6

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
String readString;
int sensPin = 12;
//int ledPin = 2;

const byte slaveId = 2;

void setup(){
  Wire.begin(slaveId); 
  Wire.onReceive(receiveEvent);
//  Wire.onRequest(requestEvent);
  
//  pinMode(ledPin,OUTPUT);
  
//  digitalWrite(ledPin,LOW);  
      
//  Serial.begin(9600);
  
//  pinMode(ledPin,OUTPUT);
//  pinMode(potPin,INPUT);
  pixels.begin(); // This initializes the NeoPixel library.  
}

void loop(){
}

void receiveEvent(int howMany){
  Serial.println("1");  
  while(Wire.available() > 0){
    delay(3);
    char c = Wire.read();  
    readString += c;
  }
  Serial.println(readString);
  if (readString.startsWith("D", 0)){
  neoPix();
  }
  readString="";  
}

void requestEvent(){
    Serial.println("1");  
  Wire.println("Hey master ");
} 

