/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Sweep
*/

#define servoA_PWM_PIN 3
#define servoB_PWM_PIN 5
#include <Servo.h>

Servo servoA;
Servo servoB;
void setup() {
  // put your setup code here, to run once:

    pinMode(servoA_PWM_PIN , OUTPUT);
    pinMode(servoB_PWM_PIN , OUTPUT);
    servoA.attach(servoA_PWM_PIN);
    servoB.attach(servoB_PWM_PIN);
}
int pos;
void loop() {
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    servoA.write(pos);              // tell servo to go to position in variable 'pos'
    servoB.write(pos);
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    servoA.write(pos);              // tell servo to go to position in variable 'pos'
    servoB.write(pos);
    delay(15);                       // waits 15ms for the servo to reach the position
  }
}
