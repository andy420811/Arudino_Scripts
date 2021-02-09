// set interrupt pin numbers to recive outhput signal:
const int sensorPin = 2; // Pin 2 as the sensor pin
long laag = 0;
long hoog = 0;
long laagTijd = 0;
long hoogTijd = 0;
void setup() {
Serial.begin(9600);
// set sensor pin as an input
pinMode(sensorPin, INPUT);
// Read starting value for millis…
while(digitalRead(sensorPin) == LOW) {;}
hoog = millis();
}
void loop(){
// Check for high and low timing – low time * 2 = PPM
while(digitalRead(sensorPin) == HIGH) {;}
laag = millis();
hoogTijd = laag – hoog;
while(digitalRead(sensorPin) == LOW) {;}
hoog = millis();
laagTijd = hoog – laag;
// Validity check high+low = 1004 (small margin or error allowed -> 990)
if(laagTijd + hoogTijd > 990)
{
Serial.print(“H: “);
Serial.println(hoogTijd,DEC);
Serial.print(“L: “);
Serial.println(laagTijd,DEC);
Serial.print(“CO2 – “);
Serial.print(laagTijd * 2,DEC);
Serial.println(” ppm”);
} else {
// Output in case of invalid check: value instead of 1004 ms
Serial.print(“– CK:”);
Serial.println(laagTijd+hoogTijd, DEC);
}
}
