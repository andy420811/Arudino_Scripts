#include <SoftwareSerial.h>
#include <Wire.h>  // Comes with Arduino IDE
// Get the LCD I2C Library here: 
// https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads
// Move any other LCD libraries to another folder or delete them
// See Library "Docs" folder for possible commands etc.


// set the LCD address to 0x27 for a 16 chars 2 line display
// A FEW use address 0x3F
// Set the pins on the I2C chip used for LCD connections:
//                    addr, en,rw,rs,d4,d5,d6,d7,bl,blpol


SoftwareSerial mySerial(A2, A3); // A0 and A1 on Arduino to RX, TX on MH-Z14 respectively

// Calibrate zero point command  
//byte cmd[9] = {0xFF,0x01,0x87,0x00,0x00,0x00,0x00,0x00,0x78};
//another calibrate zero point command, not sure which is correct
//byte cmd[9] = {0xff, 0x87, 0x87, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf2}; 

//Calibrate span point command
//byte cmd[9] = {0xFF,0x01,0x88,0x07,0xD0,0x00,0x00,0x00,0xA0};
//another calibrate span point command, not sure which is correct
//byte cmd[9] = {0xff, 0x88, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0}; 


//Reqest Gas concentration command
byte cmd[9] = {0xFF,0x01,0x86,0x00,0x00,0x00,0x00,0x00,0x79}; 
char response[9]; 
String ppmString = " ";

void setup() {
  // Run serial to connect to a computer
  Serial.begin(9600);
  // Start the serial connection to the MH-Z14
  mySerial.begin(9600);


}
void loop() 
{
  //Pinging CO2, we read, reply and translate intoPPM
  mySerial.write(cmd,9);
  mySerial.readBytes(response, 9);
  int responseHigh = (int) response[2];
  int responseLow = (int) response[3];
  int ppm = (256*responseHigh)+responseLow;

  ppmString = String(ppm); //int to string
  // Output on display
  for(int x=0;x<9;x++)
  {
    Serial.print((int)response[x], HEX);
    Serial.print(" ");
  }
  Serial.print("\n");
  //Serial.println((int)response, HEX);


  Serial.println("High:" + (String)responseHigh + " Low:" + (String)responseLow);
  Serial.print("PPM ");
  Serial.println(ppm);
  Serial.println("-----------------------------------");
  // Pause 10 seconds
  delay(1000);
}
