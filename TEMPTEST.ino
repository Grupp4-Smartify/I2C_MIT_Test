#include <Wire.h>

int ledPin = 13;
int potPin = A0;
int temp; 
const byte slaveId = 1;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(potPin,INPUT);
  Wire.begin(slaveId); 
  Wire.onRequest(requestEvent);
}

void loop() {
}
void tempRead(){
  int val;                //Create an integer variable
  val=analogRead(potPin);      //Read the analog port 0 and store the value in val
  temp=Thermister(val);   //Runs the fancy math on the raw analog value
  Serial.println(temp);    //Print the value to the serial port
  //Serial.print(" C");
  delay(1000);            //Wait one second before we do it again
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
    tempRead();
    Wire.print("123q");
    Wire.println(temp);  
  }


