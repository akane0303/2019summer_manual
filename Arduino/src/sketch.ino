#include <ros.h>
#include <std_msgs/Int16.h>
#include <std_msgs/UInt16MultiArray.h>
#include <std_msgs/Int32.h>
#include <std_msgs/Bool.h>
#include <Arduino.h>
#include <Servo.h>
#include"Motor.h"
#include"roller.h"
//
int pinT[2]={13,14};
int pinB[2]={15,16};
Motor topmotor(pinT[0],pinT[1]);
Motor bottommotor(pinB[0],pinB[1]);

//
//read A,B phase
volatile int oldEnctopB;
volatile int Enc_top_value;
volatile int oldEncbottomB;
volatile int Enc_bottom_value;

void EncoderTop(){
    float time=0;
    int newEnctopB=digitalRead(ENC_TOP_B);
    int newEnctopA=digitalRead(ENC_TOP_A);

    if(newEnctopA){//if a is high
        if(oldEnctopB && !newEnctopB){//if b is low
            ++Enc_bottom_value;
        }/*else{//if b is high
            --Enc_bottom_value;
        }
    }*/else{//if a is low
        if(!oldEnctopB && newEnctopB){//if b is high
            ++Enc_top_value;
        }/*else{//if b is high
            ++Enc_top_value;
        }*/
    }
    oldEnctopB=newEnctopB;
    time+=0.1;
}
void EncoderBottom(){
    int newEncbottomA=digitalRead(ENC_BOTTOM_A);
    int newEncbottomB=digitalRead(ENC_BOTTOM_B);
    if(newEncbottomA){
        if(!oldEncbottomB && newEncbottomB){
            ++Enc_bottom_value;
        }/*else{
            --Enc_bottom_value;
        }*/
    }else{
        if(oldEncbottomB && !newEncbottomB){
            ++Enc_bottom_value;
        }/*else{
            ++Enc_bottom_value;
        }*/
    }
    oldEncbottomB=newEncbottomB;
}
ros::NodeHandle n;

ros::Subscriber<std_msgs::Int16>sub1("move_top_motor!" &topCallback);
void topCallback(const std_msgs::INIT16& subMsg){
    //top motor run
    topmotor.run(subMsg.data);
}
ros::Subscriber<std_msgs::Int16>sub2("move_bottom_motor!" &bottomCallback);
void bottomCallback(const std_msgs::Int16& subMsg){
    //bottom motor run
    bottommotor.run(subMsg.data);
}

std_msgs::Int32 pub1_msg;
ros::Publisher pub1("top_encoder_data" &pub1_msg);//send pub1_msg to topic
std_msgs::Int32 pub2_msg;
ros::Publisher pub2("bottom_encoder_data" &pub2_msg);
int ENC_TOP_A=2;
int ENC_TOP_B=3;
int ENC_BOTTOM_A=18;
int ENC_BOTTOM_B=19;

void setup()
{
    nh.initNode();
    nh.advertise(pub1);
    nh.advertise(pub2);

    nh.subscribe(sub1);
    nh.subscribe(sub2);

    pinMode(ENC_TOP_A,INPUT);
    pinMode(ENC_TOP_B,INPUT);
    pinMode(ENC_BOTTOM_A,INPUT);
    pinMode(ENC_BOTTOM_B,INPUT);
    Enc_top_value=0;
    Enc_bottom_value=0;
    oldEnctopB=digitalRead(ENC_TOP_B);
    oldEncbottomB=digitalRead(ENC_BOTTOM_B);
    attachInterrupt(2,EncoderTop,CHANGE);
    attachInterrupt(3,EncoderTop,CHANGE)
    attachInterrupt(18,EncoderBottom,CHANGE);
    attachInterrupt(19,EncoderBottom,CHANGE);

}

void loop()
{
pub1_msg.data=Enc_top_value;
pub1.publish(&pub1_msg);
pub2_msg.data=Enc_bottom_value;
pub2.publish(&pub2_msg);

nh.spinOnce();
delay(1);


}
