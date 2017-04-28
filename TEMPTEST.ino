#include <dht.h>
#include <Wire.h>
#define dht_apin A8

dht DHT;

int hum;
int humTemp;
int potRes = 0;
String readString;

int potPin0 = A0;
int potPin1 = A1;
int potPin2 = A2;
int temp[3];
int relayPin = 2;
const byte slaveId = 3;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(potPin0,INPUT);
  pinMode(potPin1,INPUT);
  pinMode(potPin2,INPUT);
  pinMode(relayPin, OUTPUT);
  Wire.begin(slaveId); 
  Wire.onRequest(requestEvent);
  Wire.onReceive(receiveEvent);
}

void loop() {
    tempRead();
    delay(500);

    if(potRes<temp[2]){
      digitalWrite(relayPin,HIGH);      
    }else{
      digitalWrite(relayPin,LOW);      
    }

    Serial.print(hum);
    Serial.print(" %/ ");
    Serial.print(humTemp);
    Serial.print(" C/ ");
    Serial.print(temp[0]);
    Serial.print(" C/ ");
    Serial.print(temp[1]);
    Serial.print(" C/ ");
    Serial.print(temp[2]);
    Serial.println(" C/ GOLV");
    
  }
void tempRead(){
  int val;                //Create an integer variable
  val=analogRead(potPin0);      //Read the analog port 0 and store the value in val
  temp[0]=Thermister(val);   //Runs the fancy math on the raw analog value
  val=analogRead(potPin1);      //Read the analog port 0 and store the value in val
  temp[1]=Thermister(val);
  val=analogRead(potPin2);      //Read the analog port 0 and store the value in val
  temp[2]=Thermister(val);
 // temp[2]=temp[2]+10;
  
  DHT.read11(dht_apin);
  hum = DHT.humidity;
  humTemp = DHT.temperature;
  
    
}

double Thermister(int RawADC) {  //Function to perform the fancy math of the Steinhart-Hart equation
 double Temp;
 Temp = log(((10240000/RawADC) - 10000));
 Temp = 1 / (0.001129148 +(0.000234125 + (0.0000000876741 * Temp * Temp ))* Temp );
 Temp = Temp - 273.15;              // Convert Kelvin to Celsius
 //Temp = (Temp * 9.0)/ 5.0 + 32.0; // Celsius to Fahrenheit - comment out this line if you need Celsius
 return Temp;
}

void requestEvent(){
  
    Wire.print(hum);
    Wire.print("/");
    Wire.print(humTemp);
    Wire.print("/");
    Wire.print(temp[0]);
    Wire.print("/");
    Wire.print(temp[1]);
    Wire.print("/");
    Wire.print(temp[2]);
    Wire.print("/");
    
    
  }

void receiveEvent(int howMany){
  
    while(Wire.available() > 0){
    delay(3);
    char inChar = Wire.read();
    readString += inChar;
 }
    char flow[readString.length()+1];
    readString.toCharArray(flow,readString.length()+1);

    potRes = atoi(flow);
    Serial.println(potRes); 
    readString = "";
}
