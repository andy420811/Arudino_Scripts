/*
 GDXLib Demo (v. 20201123, using the 0.90 )
 This is a simple demo program for using GDX sensors on any Arduino 
 which supports the Arduino BLE library. This includes the Nano33 BLE,
 Arduino Nano33 Sense, and MKR WiFi 1010, and Arduino Uno WiFi Rev2. 
 */
#include "ArduinoBLE.h"
#include "GDXLib.h"
#include <Servo.h> //include library functions fpr servo motor
Servo myservo; //create servo object to control a servo
int sensorPin=0; //initialize global variable for pin assignment to sensor

GDXLib GDX;
char strBuffer[32]="unassigned";
 
int loopCounter=0; // used to control the time the loop runs
int loopsToGoThrough=200; // change to set how long the program runs
   
void setup(){
  Serial.begin(9600);
  delay(500);
  myservo.attach(5); //attach the servo object to Arduino pin 9 
  pinMode(9,OUTPUT); //setup the servo
  pinMode(6, OUTPUT); //setup DCU line for a servo motor 
  pinMode(7, OUTPUT); //setup DCU line for a servo motor
  pinMode(8, OUTPUT); //setup DCU line for a servo motor
  digitalWrite(6, LOW); //turn off DCU line
  digitalWrite(7, LOW); //turn off DCU line
  digitalWrite(8, LOW); //turn off DCU line
  //GDX.Begin();  //use this line for proximity pairing
      //or
  GDX.open("GDX*ACC XXXXXXXXX",1, 150);//specify device type, channel and period here 

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

  pinMode(2, OUTPUT); // output 
 
}//end of setup

void loop(){
  loopCounter++; //increment
  Serial.print(loopCounter);
  Serial.print("  ");
  float channelReading =GDX.readSensor();
  Serial.print("channelReading: ");
  Serial.print(channelReading);
  Serial.print(GDX.channelUnits());
  int servoSetting = map(channelReading, 10, 0, 0, 110); //scale outputy to angle measure (value between 0 and 179) 
  myservo.write(servoSetting); //set servo position according to scaled value 
  Serial.print(" ");
  Serial.println(servoSetting);
    if (loopCounter>loopsToGoThrough){
        Serial.print("Stopping program, and closing Bluetooth connection.");
        GDX.stop();
        GDX.close();
   }
}
