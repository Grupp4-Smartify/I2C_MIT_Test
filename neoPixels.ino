

void neoPix(){
  
  int commaIndex = readString.indexOf(',');
  //  Search for the next comma just after the first
  int secondCommaIndex = readString.indexOf(',', commaIndex + 1);
  int thirdCommaIndex = readString.indexOf(',', secondCommaIndex + 1);
  
  readString.remove(0, 1); // Remove one characters starting at index=0

  String diodValue = readString.substring(0, commaIndex); // To the end of the string
  String firstValue = readString.substring(commaIndex + 1, secondCommaIndex);
  String secondValue = readString.substring(secondCommaIndex + 1, thirdCommaIndex);
  String thirdValue = readString.substring(thirdCommaIndex + 1); // To the end of the string

  char D1=diodValue.charAt(0);
  char D2=diodValue.charAt(1);
  char D3=diodValue.charAt(2);
  char D4=diodValue.charAt(3);
  char D5=diodValue.charAt(4);
  char D6=diodValue.charAt(5);
  
  char cR[firstValue.length()+1];
  char cG[secondValue.length()+1];
  char cB[thirdValue.length()+1];
  firstValue.toCharArray(cR, firstValue.length()+1);
  secondValue.toCharArray(cG, secondValue.length()+1);
  thirdValue.toCharArray(cB, thirdValue.length()+1);

 // int d= atoi(cD);
  int r= atoi(cR);
  int g= atoi(cG);
  int b= atoi(cB);
  //char one = 1;

  if(D1=='1'){
    pixels.setPixelColor(0, pixels.Color(r,g,b));
    //Serial.println(D1);
  }
  if(D2=='1'){
    pixels.setPixelColor(1, pixels.Color(r,g,b));
  }
  if(D3=='1'){
    pixels.setPixelColor(2, pixels.Color(r,g,b));
  }
  if(D4=='1'){
    pixels.setPixelColor(3, pixels.Color(r,g,b));
  }
  if(D5=='1'){
    pixels.setPixelColor(4, pixels.Color(r,g,b));
  }
  if(D6=='1'){
    pixels.setPixelColor(5, pixels.Color(r,g,b));
  }
  pixels.show();
//  for(int i=0;i<NUMPIXELS;i++){
//   
//    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
//    pixels.setPixelColor(i, pixels.Color(r,g,b)); // Moderately bright green color.
//    pixels.setPixelColor(6, pixels.Color(46,37,255));
//    pixels.show(); // This sends the updated pixel color to the hardware.
//  }
// printRGB();
//    Serial.println(D1);
//    Serial.println(D2);
//    Serial.print(",");
//    Serial.print(g);
//    Serial.print(",");
//    Serial.println(b);
}

    
