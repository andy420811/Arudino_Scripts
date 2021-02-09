#include "SD.h"
#include "DHT.h"
#include "RTClib.h"
#define DHTPIN 9 
#define DHTTYPE DHT22
#define chipSelect 10
#define Ledpin 2
#define heaterSelPin1 3
#define heaterSelPin2 4
//#define DHTTYPE DHT22   // DHT 22 如果用的是DHT22，就用這行
//#define DHTTYPE DHT21   // DHT 21
DHT dht(DHTPIN, DHTTYPE);
RTC_DS1307 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

void setup()
{
  Serial.begin(9600);
  Serial.println("DHT22 test!");
  dht.begin();  //初始化DHT
  pinMode(Ledpin,OUTPUT);
  digitalWrite(heaterSelPin1,LOW); // Start to heat the sensor
  digitalWrite(heaterSelPin2,LOW); // Start to heat the sensor


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
} // setup()

void loop()
{

    float sensor_volt1,sensor_volt2;
    float RS_air1,RS_air2; //  Get the value of RS via in a clear air
    float sensorValue1 = 0;
    float sensorValue2 = 0;
    float h = dht.readHumidity();   //取得濕度
    float t = dht.readTemperature();  //取得溫度 
/*--- Get a average data by testing 100 times ---*/
    for(int x = 0 ; x < 100 ; x++)
    {
        sensorValue1 = sensorValue1 + analogRead(A0);
        sensorValue2 = sensorValue2 + analogRead(A1);
    }
    sensorValue1 = sensorValue1/100.0;
    sensorValue2 = sensorValue2/100.0;
/*-----------------------------------------------*/

    sensor_volt1 = sensorValue1/1024*5.0;
    sensor_volt2 = sensorValue2/1024*5.0;
    RS_air1 = sensor_volt1/(5.0-sensor_volt1); // omit *R16
    RS_air2 = sensor_volt2/(5.0-sensor_volt2); // omit *R16


  DateTime now = rtc.now();

   
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
  
  digitalWrite(Ledpin, HIGH);
  File dataFile = SD.open("datalog.csv", FILE_WRITE);
  if (dataFile) {
      dataFile.print((String)now.year() + "/" +(String)now.month()+"/"+(String)now.day()+ " " +(String)now.hour()+":"+(String)now.minute()+":"+(String)now.second()
                     + "," + (String)h + "," + (String)t + ",");
      dataFile.println((String)sensor_volt1 + "," + (String)RS_air1 );
      dataFile.println((String)sensor_volt2 + "," + (String)RS_air2 );
    dataFile.close();
    digitalWrite(Ledpin, LOW);
    // print to the serial port too:
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }
    Serial.print("sensor_volt1 = ");
    Serial.print(sensor_volt1);
    Serial.println("V");
    Serial.print("RS_air1 = ");
    Serial.println(RS_air1);

    Serial.print("sensor_volt2 = ");
    Serial.print(sensor_volt2);
    Serial.println("V");
    Serial.print("RS_air2 = ");
    Serial.println(RS_air2);
    delay(1000);
} // loop()
