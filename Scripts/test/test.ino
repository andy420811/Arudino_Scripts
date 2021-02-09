

#define motorA_DIRC_PIN 8
#define motorA_PWM_PIN 9
#define motorB_DIRC_PIN 7
#define motorB_PWM_PIN 3
int speedA,speedB,direcA,direcB;
int tmp;
void setup(){
    Serial.begin(9600);
    pinMode(motorA_DIRC_PIN,OUTPUT);
    pinMode(motorB_DIRC_PIN,OUTPUT);
    pinMode(motorA_PWM_PIN,OUTPUT);
    pinMode(motorB_PWM_PIN,OUTPUT);
    pinMode(A6,INPUT);
    pinMode(A7,INPUT);
}

void loop()
{
    while(Serial.available())
    {
        tmp = (int)(Serial.readString().toInt());
        Serial.clearWriteError();
        delay(1);
    }

        direcA = 1;
        direcB = 1;
        speedA = tmp;
        speedB = tmp;
    Serial.println((String)speedA +"  "+ (String)speedB);

    if (direcA > 0)
    {
        digitalWrite(motorA_DIRC_PIN,HIGH);
        analogWrite(motorA_PWM_PIN, speedA);
    }else
    {
        digitalWrite(motorA_DIRC_PIN,LOW);
        analogWrite(motorA_PWM_PIN, speedA);
    }
    if (direcB > 0)
    {
        digitalWrite(motorB_DIRC_PIN,HIGH);
        analogWrite(motorB_PWM_PIN, speedB);
    }else
    {
        digitalWrite(motorB_DIRC_PIN,LOW);
        analogWrite(motorB_PWM_PIN, speedB);
    }
    delay(100);
    
}