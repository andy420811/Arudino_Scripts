//引入MAX31865的函數庫
#include <Adafruit_MAX31865.h>
//設定MAX31865連接腳位:                   CS,SDI,SDO,CLK
Adafruit_MAX31865 max = Adafruit_MAX31865(11,  9, 12, 10);
// 設RREF為MAX31865 PT100 的參照電阻430歐姆
#define RREF      430.0
// 設RNOMINAL為攝氏0度時的電阻100歐姆
#define RNOMINAL  100.0
int temperature_count = 0;//溫度計數初始為0
float temperature_total = 0;//溫度總合初始為0
float temperature_offset = 0;//溫度偏移(用於校正)
void setup() {//setup()內的程式會在啟動時執行一次
  Serial.begin(115200);//開啟USB序列埠為115200baud
  max.begin(MAX31865_3WIRE);//開啟MAX31865連接，使用3線式RTD
  //要是讀到奇怪的值(小於-200度或大於500度)就改成下面這個
  //max.begin(MAX31865_4WIRE);
}
void loop() {//loop()內的程式會在setup()執行後不斷重複執行
  unsigned long time = millis();//取得毫秒數(自開機開始)
  float temperature = max.temperature(RNOMINAL, RREF);//自MAX31865取得溫度
  temperature_total+=temperature;//溫度加總
  temperature_count+=1;//計數加1
  if(temperature_count>=10){//當計數10個以上時計算平均溫度並輸出到電腦(每0.2秒採樣一次，每10個採樣平均並輸出一次，每次輸出間隔約0.2*10=2秒)
    Serial.print(time/1000.0);//印出時間(單位:秒)
    Serial.print(",");//印出分隔號
    float temperature_average=temperature_total/temperature_count;//計算平均溫度
    Serial.print(temperature_average+temperature_offset);//印出平均溫度(加上校正項)
    Serial.println();//換行
    temperature_count=0;//計數歸零
    temperature_total=0;//總合歸零
  }
  delay(200);//每次採樣之間等待200毫秒
}
