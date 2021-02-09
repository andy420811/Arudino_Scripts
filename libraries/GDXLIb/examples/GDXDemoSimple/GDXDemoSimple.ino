//GDXLibDemoSimple (v. 20201123, using the 0.90 library code)
#include "ArduinoBLE.h"
#include "GDXLib.h"
GDXLib GDX;

void setup(){
  Serial.begin(9600);
  delay(500);
  GDX.open();  //use this line for proximity pairing
      //or
  //GDX.open("GDX-ACC 0H1019K1",1, 1000);//or specify specific device, channel and period here 
     //or
  //GDX.open("GDX*ACC XXXXXXXX",1, 1000);//or specify device type, channel and period here 
  delay(100);
  Serial.print("Found: ");
  Serial.print(GDX.orderCode());
  Serial.print(" ");
  Serial.println (GDX.serialNumber());
  Serial.print("GDX.channelName() ");
  Serial.println (GDX.channelName());
  GDX.start();

  for(int row=1;row<11;row++){
     Serial.print(row);
     Serial.print(" ");
     float channelReading =GDX.readSensor();
     Serial.print(channelReading);
     Serial.print(" ");
     Serial.println(GDX.channelUnits());
   }//end of for
   GDX.stop();
}//end of setup

void loop(){
}
