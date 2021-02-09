#include "SD.h"
#include "DHT.h"
#include "RTClib.h"
#include <SoftwareSerial.h>

#define DHTPIN 9 
#define DHTTYPE DHT22
#define chipSelect 10
#define Ledpin 2
#define heaterSelPin 3

/*-----------------------use the RS_air1 and RS_air2 value in running code PreExpriment--------------------------*/

#define RS_air1 1023
#define RS_air2 1023

/*----------------------------------------------------------------------------------------------------------------*/

DHT dht(DHTPIN, DHTTYPE);
RTC_DS1307 rtc;
SoftwareSerial co2Serial1(4, 5);
SoftwareSerial co2Serial2(A2, A3);

unsigned long startTime = millis();
void setup()
{
  Serial.begin(9600);
  co2Serial1.begin(9600);
  co2Serial2.begin(9600);
  Serial.println("DHT22 test!");
  dht.begin();  //初始化DHT
  pinMode(Ledpin,OUTPUT);
  digitalWrite(heaterSelPin,LOW); // Start to heat the sensor


  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    abort();
  }

  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  
  //Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  //Serial.println("card initialized.");
} // setup()

void loop()
{
  //CO2
    int ppm_uart1,ppm_uart2;
    readCO2UART(ppm_uart1,ppm_uart2);
  //temp and wet
    
    float h = dht.readHumidity();   //取得濕度
    float t = dht.readTemperature();  //取得溫度 

  //alcohol ratio
    float sensor_volt1,sensor_volt2;
    float RS_gas1,RS_gas2; // Get value of RS in a GAS
    float ratio1,ratio2; // Get ratio RS_GAS/RS_air
    float sensorValue1 = analogRead(A0);
    float sensorValue2 = analogRead(A1);
    sensor_volt1=sensorValue1/1024*5.0;
    RS_gas1 = sensor_volt1/(5.0-sensor_volt1); // omit *R16
    
    sensor_volt2=sensorValue2/1024*5.0;
    RS_gas2 = sensor_volt2/(5.0-sensor_volt2); // omit *R16
 
  /*-Replace the name "R0" with the value of R0 in the demo of First Test -*/
    ratio1 = RS_gas1/RS_air1;  // ratio = RS/R0
    ratio2 = RS_gas2/RS_air2;  // ratio = RS/R0

  //date
  DateTime now = rtc.now();

   /*
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" (");
    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.print(") ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" %\t");
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.println(" *C ");
    Serial.print("sensor_volt1 = ");
    Serial.print(sensor_volt1);
    Serial.println("V");
    Serial.print("RS_air1 = ");
    Serial.println(RS_air1);
    Serial.print("sensor_volt2 = ");
    Serial.print(sensor_volt2);
    Serial.println("V");
    Serial.print("RS_air2 = ");
    Serial.println(RS_air2);*/
        
    //Serial.println((String)RS_air1);
    //Serial.println((String)RS_air2);

    //Serial.println("ppm = " + (String)ppm_uart1 + "," + (String)ppm_uart2);

  digitalWrite(Ledpin, HIGH);
  File dataFile = SD.open("final.csv", FILE_WRITE);
  if (dataFile) {
      dataFile.print((String)now.year() + "/" +(String)now.month()+"/"+(String)now.day()+ " " +(String)now.hour()+":"+(String)now.minute()+":"+(String)now.second()
                     + "," + (String)h + "," + (String)t + ",");
      dataFile.print((String)RS_gas1 + "," + (String)ratio1 +",");
      dataFile.print((String)RS_gas2 + "," + (String)ratio2 +",");
      dataFile.println((String)ppm_uart1 + "," + (String)ppm_uart2);
      
    dataFile.close();
    digitalWrite(Ledpin, LOW);
    // print to the serial port too:
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }
    delay(1000*6);
} // loop()

void readCO2UART(int & ppm_uart1,int & ppm_uart2){
  byte cmd[9] = {0xFF,0x01,0x86,0x00,0x00,0x00,0x00,0x00,0x79};
  byte response1[9]; // for answer
  byte response2[9];

  Serial.println("Sending CO2 request...");

  co2Serial1.listen();
  co2Serial1.write(cmd, 9); //request PPM CO2

  // clear the buffer
  memset(response1, 0, 9);
  memset(response2, 0, 9);
  int i = 0;
  while (co2Serial1.available() == 0) {
//    Serial.print("Waiting for response ");
//    Serial.print(i);
//    Serial.println(" s");
    delay(1000);
    i++;
  }
  if (co2Serial1.available() > 0 ) {
      co2Serial1.readBytes(response1, 9);
  }  
  co2Serial2.listen();
  co2Serial2.write(cmd, 9); //request PPM CO2
  while (co2Serial2.available() == 0) {
//    Serial.print("Waiting for response ");
//    Serial.print(i);
//    Serial.println(" s");
    delay(1000);
    i++;
  }
  if (co2Serial2.available() > 0 ) {
      co2Serial2.readBytes(response2, 9);
  }
  // print out the response in hexa
  for (int i = 0; i < 9; i++) {
    Serial.print(String(response1[i], HEX));
    Serial.print("   ");
  }
  Serial.println("");
  for (int i = 0; i < 9; i++) {
    Serial.print(String(response2[i], HEX));
    Serial.print("   ");
  }
  Serial.println("");
  // checksum
  /*
  byte check = getCheckSum(response1);
  
  if (response1[8] != check) {
    Serial.println("Checksum1 not OK!");
    Serial.print("Received: ");
    Serial.println(response1[8]);
    Serial.print("Should be: ");
    Serial.println(check);
  }
  check = getCheckSum(response2);
  if (response2[8] != check) {
    Serial.println("Checksum2 not OK!");
    Serial.print("Received: ");
    Serial.println(response2[8]);
    Serial.print("Should be: ");
    Serial.println(check);
  }
  */
  // ppm
  ppm_uart1 = 256 * (int)response1[2] + response1[3];
  Serial.print("PPM UART1: ");
  Serial.println(ppm_uart1);

  ppm_uart2 = 256 * (int)response2[2] + response2[3];
  Serial.print("PPM UART2: ");
  Serial.println(ppm_uart2);
  // temp
  byte temp1 = response1[4] - 40;
  Serial.print("Temperature1? ");
  Serial.println(temp1);
  byte temp2 = response2[4] - 40;
  Serial.print("Temperature2? ");
  Serial.println(temp2);
 
}

byte getCheckSum(char *packet) {
  byte i;
  unsigned char checksum = 0;
  for (i = 1; i < 8; i++) {
    checksum += packet[i];
  }
  checksum = 0xff - checksum;
  checksum += 1;
  return checksum;
}
