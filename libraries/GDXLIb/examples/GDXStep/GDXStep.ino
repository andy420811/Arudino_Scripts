/*
 GDXLib Demo (v. 20201123, using the 0.90 library code)
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
int loopCounter=0; // used to control the time the loop runs
int loopsToGoThrough=800; // change to set how long the program runs
int directionLine=4;//used to control stepper motor
int stepLine=3;//used to control stepper motor
   
void setup(){
  Serial.begin(9600);
  delay(500);
  pinMode(stepLine,OUTPUT);
  pinMode(directionLine,OUTPUT);
  //GDX.open();//use this line for proximity pairing
  GDX.open("GDX-FOR 072008Z7",2, 100);//or specify device, channel and period here 
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
 
  GDX.start();
  delay(200);

}//end of setup

void loop(){
     loopCounter++;
     digitalWrite (stepLine, LOW);
     Serial.print("LOW ");
     delay(40);
     digitalWrite (stepLine, HIGH);
     Serial.print("HIGH ");
     Serial.print(loopCounter);
     delay(40);
     Serial.print(" ");
     float channelReading =GDX.readSensor();
     Serial.print(channelReading);
     Serial.print("directionLine: ");
     if (channelReading>0){
      digitalWrite(directionLine, HIGH);
      Serial.println(" HIGH");
     }
     else digitalWrite {
      digitalWrite(directionLine, LOW);
      Serial.println(" LOW");
     }
   
     if (loopCounter>loopsToGoThrough){
      Serial.print("Stopping program, and closing Bluetooth connection.");
       GDX.stop();
       GDX.close();
      }
}
