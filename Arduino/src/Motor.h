#ifndef MOTOR_H
#define MOTOR_H

#include<Arduino.h>

class Motor{
    public:
        Motor(int pin1,int pin2);
        void run (int speed);
    private:
        int pin1_;
        int pin2_;
};
Motor::Motor(int pin1,int pin2){
    pin1_=pin1;
    pin2_=pin2;
    pinMode(pin1_,OUTPUT);
    pinMode(pin2_,OUTPUT);

}
void Motor::run(int speed){
    if(speed>=0){
        analogWrite(pin1_,speed);
        digitalWrite(pin2_,LOW);
    }
    if(speed<=0){
        digitalWrite(pin1_,LOW);
        analogWrite(pin2_,-speed);
    }
}
#endif
