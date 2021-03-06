/*
 GDXLib Demo (v. 20201220, using the 0.90 library code)
 This is a simple demo program for using GDX sensors on any Arduino 
 which supports the Arduino BLE library. This includes the Nano33 BLE,
 Arduino Nano33 Sense, and MKR WiFi 1010, and Arduino Uno WiFi Rev2.
 The lines with the major indents are for using a 2-line display connected
 with four I2C lines. 
 */
#include "ArduinoBLE.h"
#include "GDXLib.h"
GDXLib GDX;
char strBuffer[32]="unassigned";
char strUnits[32]="strUnits";
   //indented lines are for 2-line I2C display
   // this version uses I2C connection
   #include <Wire.h>
   #include <SerLCD.h> //Click here to get the library: http://librarymanager/All#SparkFun_SerLCD
   SerLCD lcd; // Initialize the library with default I2C address 0x72

void setup(){
  Serial.begin(9600);
  delay(500);
      // 2-LINE DISPLAY CODE
      Wire.begin();
      lcd.begin(Wire); //Set up the LCD for I2C communication
      lcd.setBacklight(255, 255, 255); //Set backlight to bright white
      lcd.setContrast(5); //Set contrast. Lower to 0 for higher contrast.
      lcd.clear(); //Clear the display - this moves the cursor to home position as well
      lcd.print("Searching for");
      lcd.setCursor(0,1);// column, row
      lcd.print("GDX Sensor");
      delay(1000);
      //2-LINE DISPLAY CODE*/
  GDX.open();  //use this line for proximity pairing
      //or
  //GDX.open("GDX-ACC 0H1019K1",1, 1000);//or specify specific device, channel and period here 
      //or
  //GDX.open("GDX*ACC XXXXXXXX",1, 1000);//or specify device type, channel and period here 
  Serial.print("Found: ");
  Serial.print (GDX.orderCode());
  Serial.print(" ");
  Serial.println (GDX.serialNumber());
  
  Serial.print("channelName; ");
  Serial.println (GDX.channelName());

  Serial.print("channelUnits: ");
  Serial.println (GDX.channelUnits());
  
  Serial.print("channelNumber: ");
  Serial.println (GDX.channelNumber());
  
  Serial.print("Battery status(%): ");
  Serial.println (GDX.batteryPercent());
  
  Serial.print("ChargeState: ");
  Serial.print (GDX.chargeState());
  Serial.println(" (0=idle, 1=charging, 2=charging complete, 3=error)");
  
  Serial.print("Bluetooth signal strength (RSSI): "); 
  Serial.println (GDX.RSSI());
  Serial.println ("threshold = -44 ");//!!!NOTE SET IN .CPP CODE
  
  Serial.print("samplePeriod: ");
  Serial.println (GDX.samplePeriodInMilliseconds());
  // 2-LINE DISPLAY CODE
      lcd.clear();
      lcd.print("Found: ");
      lcd.setCursor(0,1);// column, row
      lcd.print (GDX.orderCode());
      lcd.print(" ");
      lcd.println (GDX.serialNumber());
      delay(2000);
      
      lcd.clear();
      lcd.print(GDX.channelName());
      lcd.setCursor(0,1);// column, row
      strcpy(strUnits,GDX.channelUnits());
      ConvertUTF8ToASCII(strUnits);//used to handle degree sign and superscripts
      lcd.print(strUnits);
      delay(2000);
            
      lcd.clear();
      sprintf(strBuffer,"%s %d %s %d","batt:",GDX.batteryPercent(),"CS:",GDX.chargeState());
      lcd.print(strBuffer);
      sprintf(strBuffer,"%s %d", "channelNumber",GDX.channelNumber());
      lcd.setCursor(0,1);// column, row
      lcd.print(strBuffer);
      delay(2000);
     
      lcd.clear();
      sprintf(strBuffer,"%s %d", "   RSSI:   ",GDX.RSSI());
      lcd.print(strBuffer);
      lcd.setCursor(0,1);// column, row
      lcd.print("threshold = -44 ");//!!!NOTE SET IN .CPP CODE
      delay(2000);
      //2-LINE DISPLAY CODE*/
  
  GDX.start();
  delay(200);
  for(int row=1;row<11;row++){
     Serial.print(row);
     Serial.print(" ");
     float channelReading =GDX.readSensor();
     Serial.print(channelReading);
     Serial.print(" ");
     Serial.println (GDX.channelUnits());
       // 2-LINE DISPLAY CODE
       lcd.clear(); //Clear 
       // The probably imperfect and confusing code below is needed because the 
       // Arduino IDE does not support sprintf used with floating point numbers.
       if (channelReading>=0){
         sprintf(strBuffer," %d.%02d", (int)channelReading, (int)(channelReading*100)%100); 
       }   
       else {
          sprintf(strBuffer,"- %d.%02d", (int)channelReading*-1, (int)(channelReading*-1*100)%100); 
       }
       lcd.print(strBuffer);//note strBuffer was assigned above
       lcd.setCursor(0,1);// column, row
       lcd.print(strUnits);
       //2-LINE DISPLAY CODE */
   }//end of for
   GDX.stop();
   Serial.println("series of readings taken; stopping GDX data collection");
}//end of setup

void loop(){
}

void ConvertUTF8ToASCII(char* s)
{
  unsigned int k = 0;
  unsigned int len = strlen(s);
  byte c;

  for (unsigned int i = 0; i < len; i++)
  {
    c = s[i];
    if (c == 0xC2)
    {
      i++; // skip to the next character
      c = s[i];
      if      (c == 0xB5) c = 'u';  // micro
      else if (c == 0xB0) c = 0xDF; // degrees (specific for 16x2 LCD character set)
      else if (c == 0xB2) c = '2';  // squared (superscript 2)
      else if (c == 0xB3) c = '3';  // cubed (superscript 3)
      //else if (c == 0xB2) c = '2';  // subscript 2 !!! what is sent for subscript 2????
    }

    s[k] = c;
    k++;
  }
  s[k] = 0;
}
