#include <LiquidCrystal.h> 
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

float V;
int count = 0;

String readString;

const byte slaveId = 88;

void setup(){
  
  lcd.createChar(0, sun1);
  lcd.createChar(1, sun2);
  lcd.createChar(2, sun3);
  lcd.createChar(3, sun4);
  lcd.createChar(4, tracking1);
  lcd.createChar(5, tracking2);
  lcd.createChar(6, tracking3);

  pinMode(A0, INPUT);
  
  Wire.begin(slaveId);
  Wire.onReceive(receiveEvent);
  Serial.begin(9600);  

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
  standbyPrint();
  Serial.begin(9600); 
  Wire.begin();

  printCurrent();
  
}

void loop(){ 
   V = voltage();
   //lcd.setCursor(8,1);
   //lcd.print("   ");
   if(V>=5){
    lcd.setCursor(8,1);
    lcd.print("Max");
   }else{ 
     lcd.setCursor(8,1);
     lcd.print(V,1);
   }
   
}

float voltage(){
  float v,a,vTot,mArray[18],temp;
  a = (5.0/1023.0);
  
  for(int i=0;i<18;i++){
  v = analogRead(A0);
  mArray[i] = v*a;
  }
  for(int m=0;m<18;m++){
    for(int n=0;n<m-1;n++){
      if(mArray[n]>mArray[n+1]){
        temp = mArray[n];
        mArray[n] = mArray[n+1];
        mArray[n+1] = temp;
      }
    }
  } 
  count++;
  if(count==300){
    count = 0;
    return mArray[9];
  }
}

void printCurrent(){  
   lcd.setCursor(0,1);
   delay(1);
   lcd.print("              ");
   lcd.setCursor(0,1);
   delay(1);
   lcd.print("Current:");
   lcd.setCursor(11,1);
   delay(1);
   lcd.print("V");
   lcd.print("  ");
}

void standbyPrint(){ 
   lcd.setCursor(0,0);
   delay(1);
   lcd.print("            ");
   lcd.setCursor(0,0);
   lcd.print("Standby       ");
   printCurrent();
   printSun();
}

void autoPrint(){  
   lcd.setCursor(0,0);
   lcd.print("            ");
   lcd.setCursor(0,0);
   delay(1);
   lcd.print("Auto          ");
   printCurrent();
   printSun();
}

void manualPrint(){  
   lcd.setCursor(0,0);
   delay(1);
   lcd.print("             ");
   lcd.setCursor(0,0);
   delay(1);
   lcd.print("Manual        ");
   printCurrent();
   printSun();
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

void receiveEvent(int howMany){
char inChar;  
  while(Wire.available()>0){ 
    inChar = Wire.read();  
  } 
  if(inChar == 's'){    
    Serial.println("Standby");
    standbyPrint();
  }
  if(inChar == 'a'){    
    Serial.println("Auto"); 
    autoPrint();
  }
  if(inChar == 'm'){    
    Serial.println("Manual"); 
    manualPrint();
  } 
}
