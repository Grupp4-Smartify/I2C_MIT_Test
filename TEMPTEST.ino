#include <dht.h>
#include <Wire.h>
#define dht_apin A8

dht DHT;

int potPin0 = A0;
int potPin1 = A1;
int potPin2 = A2;
int potPin3 = A3;
int temp[4];
const byte slaveId = 1;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(potPin0,INPUT);
  pinMode(potPin1,INPUT);
  pinMode(potPin2,INPUT);
  pinMode(potPin3,INPUT);
  Wire.begin(slaveId); 
  /*Wire.onRequest(requestEvent);
  Wire.onReceive(receiveEvent);*/
}

void loop() {
    tempRead();
  for(int i=0;i<4;i++){
    
    Serial.print(" Temp");
    Serial.print(i);
    Serial.print(" = ");
    Serial.print(temp[i]);
    Serial.print(" | ");
  }
  Serial.println();
  delay(2000);

}
void tempRead(){
  int val;                //Create an integer variable
  val=analogRead(potPin0);      //Read the analog port 0 and store the value in val
  temp[0]=Thermister(val);   //Runs the fancy math on the raw analog value
  val=analogRead(potPin1);      //Read the analog port 0 and store the value in val
  temp[1]=Thermister(val);
  val=analogRead(potPin2);      //Read the analog port 0 and store the value in val
  temp[2]=Thermister(val);
  val=analogRead(potPin3);      //Read the analog port 0 and store the value in val
  temp[3]=Thermister(val);
  
  DHT.read11(dht_apin);
    
  Serial.print("Current humidity = ");
  Serial.print(DHT.humidity);
  Serial.print("%  ");
    
}

double Thermister(int RawADC) {  //Function to perform the fancy math of the Steinhart-Hart equation
 double Temp;
 Temp = log(((10240000/RawADC) - 10000));
 Temp = 1 / (0.001129148 +(0.000234125 + (0.0000000876741 * Temp * Temp ))* Temp );
 Temp = Temp - 273.15;              // Convert Kelvin to Celsius
 //Temp = (Temp * 9.0)/ 5.0 + 32.0; // Celsius to Fahrenheit - comment out this line if you need Celsius
 return Temp;
}

/*void requestEvent(){
  for(int i=0;i>3;i++){
    tempRead(i);
    Wire.print("123q");
    Wire.println(temp); 
   } 
  }

void receiveEvent(){
    
  }*/
