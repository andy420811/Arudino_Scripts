/*
 GDXLib Demo (v. 20201123, using the 0.90 )
 This is a simple demo program for using GDX sensors on any Arduino 
 which supports the Arduino BLE library. This includes the Nano33 BLE,
 Arduino Nano33 Sense, and MKR WiFi 1010, and Arduino Uno WiFi Rev2.
 This program controls three LED, sort of like a traffic light, based on 
 sound level. It supports and I2C 2-line display.
 */
#include "ArduinoBLE.h"
#include "GDXLib.h"
GDXLib GDX;
char strBuffer[32]="unassigned";

void setup(){
  Serial.begin(9600);
  delay(500);
  pinMode(2, OUTPUT); // output 
  pinMode(3, OUTPUT); // output 
  pinMode(4, OUTPUT); // output 
  pinMode(5, OUTPUT); // output 
   
  //GDX.open("GDX-SND 0T1001J2",2, 200);//specify specific device, channel and period here 
  GDX.open("GDX*SND XXXXXXXX",2, 200);//or device type, channel and period here 

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
}//end of setup

void loop(){
  float channelReading =GDX.readSensor();
  Serial.print("channelReading: ");
  Serial.print(channelReading);
  Serial.println(GDX.channelUnits());
  if (channelReading<70) digitalWrite (2,HIGH);
  else digitalWrite(2, LOW);
  if (channelReading>70) digitalWrite (3,HIGH);
    else digitalWrite(3, LOW);
  if (channelReading>90) digitalWrite (4,HIGH);
    else digitalWrite(4, LOW);
  digitalWrite(5, LOW);
}
