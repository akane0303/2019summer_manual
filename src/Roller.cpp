#include<ros/ros.h>
#include<std_msgs/Int32.h>
#include<std_msgs/Int16.h>

/*
 * subscrive(ENC_TOP_VAL,10,callback)
 *callback(){
 *  kakudo =msg_data/120*pi
 * }
 *void calc(){
    kakudo-oldkakudo/0.001;
    kakudo=oldkaludo
    kakudo_msg_data=kakudo;
    pub.publish(kakudo_msg)
 }
*/
class Roller{
    public:
        Roller();
        void publish();

    private:
        void rollertopcallback(const std_msgs::Int16::ConstPtr& sub_msg);      
        void rollerbottomcallback(const std_msgs::Int16::ConstPtr& sub_msg);
        ros::NodeHandle nh;
        ros::Subscriber topsub;
        ros::Subscriber bottomsub;

        ros::Publisher Toppub;
        ros::Publisher Bottompub;

        long  count;
        double angle,speed,oldangle;
        double kp;
        int target;
};

Roller::Roller(){
    topsub=nh.subscribe<std_msgs::Int16>("top_encoder_data",10,&Roller::rollertopcallback,this);
    bottomsub=nh.subscribe<std_msgs::Int16>("bottom_encoder_data",10,&Roller::rollerbottomcallback,this);


    Toppub=nh.advertise<std_msgs::Int16>("move_top_motor!",1);
    Bottompub=nh.advertise<std_msgs::Int16>("move_bottom_motor!",1);

    count=0;
    angle=0;
    speed=0;
    oldangle=0;
    kp=1;
    target=255;
}

void Roller::rollertopcallback(const std_msgs::Int16::ConstPtr& sub_msg){
    count=sub_msg->data;
}
void Roller::rollerbottomcallback(const std_msgs::Int16::ConstPtr& sub_msg){
    count=sub_msg->data;
}

void Roller::publish(){
    std_msgs::Int16  move_top_motor_msg;
    std_msgs::Int16  move_bottom_motor_msg;

    angle=(count/120)*360;
    speed=(angle-oldangle)/0.001;
    
    move_top_motor_msg.data=kp*(target-speed);
    move_bottom_motor_msg.data=kp*(target-speed);
    

    std::cout<<"roller top:"<<move_top_motor_msg.data<<std::endl;
    std::cout<<"roller bottom:"<<move_bottom_motor_msg.data<<std::endl;
    Toppub.publish(move_top_motor_msg);
    Bottompub.publish(move_bottom_motor_msg);

    oldangle=angle;
}

int main(int argc,char **argv){
    ros::init(argc,argv,"roller");
    Roller roller;
    ros::Rate loop_rate(10);
    while(ros::ok()){
        roller.publish();
        ros::spinOnce();
        loop_rate.sleep();
    }
    return 0;
}
