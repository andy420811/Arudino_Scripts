
#define motorA_DIRC_PIN 8
#define motorA_PWM_PIN 9

void setup() {
  // put your setup code here, to run once:

    pinMode(motorA_DIRC_PIN , OUTPUT);
    pinMode(motorA_PWM_PIN , OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  
        digitalWrite(motorA_DIRC_PIN, LOW);
        analogWrite(motorA_PWM_PIN , 128);
}
