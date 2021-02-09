#include<PS2X_lib.h>
//#include<LedControl.h>
#include<Servo.h>


#define motorA_DIRC_PIN 8
#define motorA_PWM_PIN 9
#define motorB_DIRC_PIN 7
#define motorB_PWM_PIN 6
#define servoA_PWM_PIN 3
#define servoB_PWM_PIN 5
#define Voltage_meter_PIN A0

int speedA,speedB; 
int roll,pitch;
float signalA,signalB;
float battery;
bool activate_L1mode;

bool mode;
int16_t tmp[6];
int16_t last_speed[2];
uint16_t count;
bool last_commend;
byte error;
PS2X ps2x; 
//LedControl lc=LedControl(A7,A6,A5,1);
Servo servoA;
Servo servoB;

void setup()
{
    
    Serial.begin(9600);
    //配對接收器 
    do { 
        //GamePad(clock, command, attention, data, Pressures?, Rumble?)
        error = ps2x.config_gamepad(13, 11, 10, 12, true, true);   //這行要和接線對應正確
        if (error == 0) { Serial.print("Gamepad found!");break; } 
        else { delay(100); } 
    } while (1); 
    
    pinMode(motorA_DIRC_PIN , OUTPUT);
    pinMode(motorA_PWM_PIN , OUTPUT);
    pinMode(motorB_DIRC_PIN , OUTPUT);
    pinMode(motorB_PWM_PIN , OUTPUT);

    servoA.attach(servoA_PWM_PIN);
    servoB.attach(servoB_PWM_PIN);

    memset(tmp ,0 ,sizeof(tmp));
    memset(last_speed, 0, sizeof(last_speed));
}
void loop()
{
    ps2x.read_gamepad(false, 0);
    

    mode = Control_Mode();
    Read_commend(true);
    Check_Change();
    MotorA_Act();
    MotorB_Act();
    
}

byte Check_Change(){
    /*throttle count*/
    tmp[0] == speedA ? count += 0B0000000001 : count &= 0B1111111100;
    if(count & 0B0000000011)count &= 0B11111100;
    tmp[1] == speedB ? count += 0B0000000100 : count &= 0B1111110011;
    if(count & 0B0000001100)count &= 0B1111110011;

    /*mode count*/
    (last_commend == activate_L1mode)? count += 0B0000010000 : count &= 0B1111001111;
    if(count & 0B0000110000)count &= 0B111111001111;

    /*direction count*/
    tmp[2] == roll  ?  count += 0B0001000000 : count &= 0B1100111111;
    if(count & 0B0011000000)count &= 0B1100111111;
    tmp[3] == pitch ?  count += 0B0100000000 : count &= 0B0011111111;
    if(count & 0B1100000000)count &= 0B0011111111;

    tmp[0] = speedA; 
    tmp[1] = speedB;
    tmp[2] = roll;
    tmp[3] = pitch;
    last_commend = activate_L1mode;

    return count;
}

void HandlerToSignal(int speed , float* signal){
    *signal = abs(speed > 0 ? -speed *2 + 256 : map(-speed * 2, 0 , 254 , 0 , 256));
    *signal = pow(*signal / 16, 2);

}
/*
void Display_battery(){
    static bool Start = false;
    static uint8_t count2 = 0;
    volatile int i;
    if(!Start){
        lc.shutdown(0,false);
        // Set the brightness to a medium values 
        lc.setIntensity(0,0.00003);
        // and clear the display 
        lc.clearDisplay(0);
    }else{
        if(count2 == 10){
            battery = (float)analogRead(Voltage_meter_PIN) / 1023;
            Serial.println(battery*25);
            lc.clearDisplay(0);
            for(i = 0 ;i < battery * 25; i++){
                
                lc.setLed(0 , i/5 , i%5 , true);
            }
            count2 = 0;
        }
        if(count2 == 5){
            i--;
            lc.setLed(0, i/5 , i%5 , false);
        }
        count2++;
    }
}
*/
bool Control_Mode(){
    bool separte_mode;
    /*if the button L1 is pressed the separte motor control will be activate*/
    if((count >> 4) > 0b000000010 ){
        separte_mode = true;
    }else{
        separte_mode = false;
    }
    return separte_mode;
}
void Read_commend(bool mode){
    int L1_value,R1_value;
    activate_L1mode = (ps2x.Button(PSB_L1) == true);
    if(mode){
        speedA = ps2x.Analog(PSS_LY) - 127;
        speedB = ps2x.Analog(PSS_RY) - 127;
        L1_value = (int)ps2x.Analog(PSB_L1);
        R1_value = (int)ps2x.Analog(PSB_R1);
        pitch = (R1_value - L1_value)/2;
        roll = ps2x.Analog(PSS_RX) - 127;
    }else{
        speedB = speedA = ps2x.Analog(PSS_LY) - 127;
        pitch = ps2x.Analog(PSS_RY) - 127;
        roll = ps2x.Analog(PSS_RY) - 127;
    }
}

void MotorA_Act(){
    if(count > 0b000000010)
    {
        digitalWrite(motorA_DIRC_PIN, last_speed[0] > 0 ? HIGH : LOW );
        digitalWrite(motorB_DIRC_PIN, speedB > 0 ? HIGH : LOW );

        HandlerToSignal(last_speed[0],&signalA);
        HandlerToSignal(speedB, &signalB);

        analogWrite(motorA_PWM_PIN , signalA);
        analogWrite(motorB_PWM_PIN , signalB);

        last_speed[1] = speedB;

        //Serial.print((String)"First: "+ (String)" speedA: "+ (String)(last_speed[0] > 0 ? " HIGH " : " LOW ")  + (String)signalA + " SpeedB: " + (String)(speedB > 0 ?  " HIGH " : " LOW ")  +(String)signalB);
        //Serial.println(" speedA_now: "+ (String)speedA);
    }
}
void MotorB_Act(){
    if((count >> 2) > 0b0000010){
        digitalWrite(motorA_DIRC_PIN, speedA > 0 ? HIGH : LOW );
        digitalWrite(motorB_DIRC_PIN, last_speed[1] > 0 ? HIGH : LOW );


        HandlerToSignal(speedA, &signalA);
        HandlerToSignal(last_speed[1], &signalB);

        analogWrite(motorA_PWM_PIN , signalA);
        analogWrite(motorB_PWM_PIN , signalB);
        last_speed[0] = speedA;

        //Serial.print((String)"Second: " + (String)" speedA: "+ (String)(speedA > 0 ? " HIGH " : " LOW ")   + (String)signalA + " SpeedB: " + (String)(last_speed[1] > 0?" HIGH " : " LOW ") +(String)signalB);
        //Serial.println(" speedB_now: "+ (String)speedB);
    }
}
void ServoA_Act(){
    if((count >> 6) > 0b0000010){
        signalA = map(roll, -128, 128, 0, 180);
        servoA.write(signalA);
    }
}
void ServoB_Act(){
    if((count >> 8) > 0b0000010){
        signalB = map(pitch, -128, 128, 0, 180);
        servoA.write(signalB);
    }
}
