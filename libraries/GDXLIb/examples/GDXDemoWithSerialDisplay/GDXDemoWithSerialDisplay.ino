/*
 GDXLib Demo (v. 20201212, using the 0.90 library code)
 This is a simple demo program for using GDX sensors on any Arduino 
 which supports the Arduino BLE library. This includes the Nano33 BLE,
 Arduino Nano33 Sense, and MKR WiFi 1010, and Arduino Uno WiFi Rev2.
 The lines with the major indents are for using a 2-line display connected
 with three serial lines. 
 */
#include "ArduinoBLE.h"
#include "GDXLib.h"
GDXLib GDX;
char strBuffer[32]="unassigned";

void setup(){
  Serial.begin(9600);
  delay(500);
      // 2-LINE DISPLAY CODE
      CharDisplayInit();
      CharDisplayPrintLine(1,"Searching for");
      CharDisplayPrintLine(2,"GDX sensor");
      delay (2000);
      //2-LINE DISPLAY CODE*/
  Serial.println("Searching for");
  Serial.println("GDX Sensor");
  GDX.open();  //use this line for proximity pairing
      //or
  //GDX.open("GDX-FOR 072001D0",1, 500);//or specify specific device, channel and period here 
     //or
  //GDX.open("GDX*ACC XXXXXXXX",1, 1000);//or specify device type, channel and period here 
  Serial.print("Found: ");
  Serial.print (GDX.orderCode());
  Serial.print (" ");
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
      CharDisplayPrintLine(1,"Found: ");
      delay(1000);
      CharDisplayPrintLine (1,GDX.orderCode());
      CharDisplayPrintLine (2,GDX.serialNumber());
      delay(2000);
      
      CharDisplayPrintLine (1,GDX.channelName());
      CharDisplayPrintLine (2,GDX.channelUnits());
      delay(2000);
            
      sprintf(strBuffer,"%s %d %s %d","batt:",GDX.batteryPercent(),"CS:",GDX.chargeState());
      CharDisplayPrintLine(1, strBuffer);
      sprintf(strBuffer,"%s %d", "channelNumber",GDX.channelNumber());
      CharDisplayPrintLine(2, strBuffer);
      delay(2000);
     
      sprintf(strBuffer,"%s %d", "   RSSI:   ",GDX.RSSI());
      CharDisplayPrintLine (1,strBuffer);
      CharDisplayPrintLine (2,"threshold = -44 ");//!!!NOTE SET IN .CPP CODE
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
     Serial.println(GDX.channelUnits());
     sprintf(strBuffer, "%.2f", channelReading);
       // 2-LINE DISPLAY CODE
       // The probably imperfect and confusing code below is needed because the 
       // Arduino IDE does not support sprintf used with floating point numbers.
       if (channelReading>=0){
         sprintf(strBuffer," %d.%02d", (int)channelReading, (int)(channelReading*100)%100); 
       }   
       else {
          sprintf(strBuffer,"- %d.%02d", (int)channelReading*-1, (int)(channelReading*-1*100)%100); 
       }
       CharDisplayPrintLine (1,strBuffer);
       CharDisplayPrintLine (2,GDX.channelUnits());
     //2-LINE DISPLAY CODE */
   }//end of for
   GDX.stop();
   Serial.println("series of readings taken; stopping GDX data collection");
}//end of setup

void loop(){
}

// All the functions below are used for TWO_LINE_DISPLAY with serial connection.
      
void CharDisplayPrintLine(int line, const char* strText){
    uint8_t lineCode = 128;
    if (line == 2) lineCode = 192;
  
    // Force a field width of 16.
    // Left justified with space padding on the right.
    //char strBuffer[32];
    sprintf(strBuffer, "%-16.15s",  strText);
  
    //For debug -- prints what goes to the display
    //Serial.print(" strBuffer" ");
    //Serial.print("[");
    //Serial.print(strBuffer);
    //Serial.println("]");

    ConvertUTF8ToASCII(strBuffer);// to handle degree sign, and superscripts
    Serial1.write((uint8_t)254); 
    Serial1.write(lineCode); 
    Serial1.write(strBuffer, 15);
}
      
void CharDisplayInit(){
    Serial1.begin(9600);
    delay(500);
    
    //Clear the screen
    Serial1.write((uint8_t)254);
    Serial1.write((uint8_t)1);
    // Move cursor to beginning of line1
    Serial1.write(254); 
    Serial1.write(128);  
}

void ConvertUTF8ToASCII(char* s) // to handle degree sign, and superscripts
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
     }

    s[k] = c;
    k++;
   }
   s[k] = 0;
 }
