#include "SD.h"
#include "DHT.h"
#include "RTClib.h"

#define heaterSelPin 3
#define DHTPIN 9 
#define DHTTYPE DHT22
#define chipSelect 10
#define Ledpin 2
DHT dht(DHTPIN, DHTTYPE);
RTC_DS1307 rtc;

void setup() {
    Serial.begin(9600);
    pinMode(heaterSelPin,OUTPUT);   // set the heaterSelPin as digital output.
    digitalWrite(heaterSelPin,LOW); // Start to heat the sensor
    pinMode(Ledpin,OUTPUT);
    dht.begin();  //初始化DHT

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    abort();
  }

  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  } 

  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }

  Serial.println("card initialized.");
}
 
void loop() {
    float sensor_volt;
    float RS_air; //  Get the value of RS via in a clear air
    float sensorValue = 0;
    float h = dht.readHumidity();   //取得濕度
    float t = dht.readTemperature();  //取得溫度 
/*--- Get a average data by testing 100 times ---*/ 
    for(int x = 0 ; x < 100 ; x++)
    {
        sensorValue = sensorValue + analogRead(A0);
    }
    sensorValue = sensorValue/100.0;
/*-----------------------------------------------*/

    sensor_volt = sensorValue/1024*5.0;
    RS_air = sensor_volt/(5.0-sensor_volt); // omit *R16
    DateTime now = rtc.now();

   Serial.print(now.year(), DEC);
   Serial.print('/');
   Serial.print(now.month(), DEC);
   Serial.print('/');
   Serial.print(now.day(), DEC);
   Serial.print(" (");
    Serial.print(") ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
   digitalWrite(Ledpin, HIGH);
  File dataFile = SD.open("Data.csv", FILE_WRITE);
  if (dataFile) {
    if(RS_air < 10){

      //Data series : time,wet,temp,sensor_volt,RS_air
      dataFile.print((String)now.year() + "/" +(String)now.month()+"/"+(String)now.day()+ " " +(String)now.hour()+":"+(String)now.minute()+":"+(String)now.second()
                     + "," + (String)h + "," + (String)t + ",");
      dataFile.println((String)sensor_volt + "," + (String)RS_air );
    }
    dataFile.close();
    digitalWrite(Ledpin, LOW);
    // print to the serial port too:
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }
    Serial.print("sensor_volt = ");
    Serial.print(sensor_volt);
    Serial.println("V");
    Serial.print("RS_air = ");
    Serial.println(RS_air);
    delay(1000);
}
