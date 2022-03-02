#include <PID_v1.h>
#include <Encoder.h>
#include <ros.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Vector3Stamped.h>

#define LEFT 0
#define RIGHT 1
#define LOOPTIME      100

//Pin config
//const int EN1_LEFT = 40;
//const int EN2_LEFT = 42;
const int DIR1_LEFT = 9;
const int DIR2_LEFT = 10;
//Fixing reversed direction
const int PWM_LEFT = 8;

///const int EN1_RIGHT = 41;
///const int EN2_RIGHT = 43;
const int DIR1_RIGHT = 6;
const int DIR2_RIGHT = 7;
const int PWM_RIGHT = 5;

//Computation variables
const double PID_Left_Param[3] = {2.5, 2.5, 0.1};
const double PID_Right_Param[3] = {2.5, 2.5, 0.1};

// Experimental: Ubah PID sesuai arah putar motor
// const double PID_Backward_Param[] = {30.0, 2.0, 0.1};
// const double PID_Forward_Param[]= {2.5, 2.5, 0.1};

int PWM_leftMotor = 0;
int PWM_rightMotor = 0;
int led_status_fade;

double linear_speed_sp = 0;
double angular_speed_sp = 0;

double left_motor_sp = 0;
double right_motor_sp = 0;

double left_corr_spd = 0;
double left_act_spd = 0;

double right_corr_spd = 0;
double right_act_spd = 0;

double lastMillis;
unsigned long lastMillis2;

bool left_reverse, right_reverse;

//Debug variables
bool ros_connect_status = true;
double last_val_1, last_val_2;

const byte noVelDataMax = 10;
unsigned int noVelData = 0;

//Robot physical constraints
const double min_speed_reverse_cmd = 0.031168599; //Adjustment for wheelchair motor
const double wheel_dia = 0.32; //Wheel radius, in m
const double wheelbase = 0.57; //Wheelbase, in m
const double encoder_cpr = 800; //Encoder ticks or counts per rotation
const double speed_to_pwm_ratio = 0.0123677; //Ratio to convert speed (in m/s) to PWM value. It was obtained by plotting the wheel speed in relation to the PWM motor command (the value is the slope of the linear function).
const double wheel_gear_ratio = 26.26; //Gearbox ratio
const double max_speed = 0.7;
const double min_speed_cmd = 0.039;          //(min_speed_cmd/speed_to_pwm_ratio) is the minimum command value needed for the motor to start moving. This value was obtained by plotting the wheel speed in relation to the PWM motor command (the value is the constant of the linear function).
const double brake_power = 150; //Experimental for smoother braking

PID PID_left_motor(&left_act_spd, &left_corr_spd, &left_motor_sp, 
                   PID_Left_Param[0], PID_Left_Param[1], PID_Left_Param[2], DIRECT);
PID PID_right_motor(&right_act_spd, &right_corr_spd, &right_motor_sp, 
                   PID_Right_Param[0], PID_Right_Param[1], PID_Right_Param[2], DIRECT);

void vel_handle (const geometry_msgs::Twist& cmd_vel){
  //Update noVelData
  noVelData = 0;
  
  linear_speed_sp = cmd_vel.linear.x;
  angular_speed_sp = cmd_vel.angular.z;

  left_motor_sp = linear_speed_sp - angular_speed_sp * (wheelbase/2);
  right_motor_sp = linear_speed_sp + angular_speed_sp * (wheelbase/2);

  // Experimental: Ubah PID sesuai arah putar motor
  //Changing PID params according to the direction of the motor
  // if(left_motor_sp < 0){
  //   // Change PID to backward PID
  //   PID_left_motor.SetTunings(PID_Backward_Param[0], PID_Backward_Param[1], PID_Backward_Param[2]);
  // }
  // else{
  //   // Change PID to forward PID
  //   PID_left_motor.SetTunings(PID_Forward_Param[0], PID_Forward_Param[1], PID_Forward_Param[2]);
  // }

  // if(right_motor_sp < 0){
  //   // Change PID to backward PID
  //     PID_right_motor.SetTunings(PID_Backward_Param[0], PID_Backward_Param[1], PID_Backward_Param[2]);
  // }
  // else{
  //   // Change PID to forward PID
  //     PID_right_motor.SetTunings(PID_Forward_Param[0], PID_Forward_Param[1], PID_Forward_Param[2]);
  // }

  // Adjustment for wheelchair motor reverse speed
  if(left_motor_sp < 0){
    left_reverse = true;
  }
  else{ left_reverse = false; }

  if(right_motor_sp < 0){
   right_reverse = true; 
  }
  else{ right_reverse = false; }
}

void setMotorSpeed(int i, int spd);
void initMotor();
void StopBothMotor();
float measureSpeed(long count, long time1);
void publishSpeed(double time);
template <typename T> int sgn(T val);

//Object creating
ros::NodeHandle nh;
ros::Subscriber <geometry_msgs::Twist> cmd_vel("cmd_vel", vel_handle);
geometry_msgs::Vector3Stamped speed_msg;
ros::Publisher vel_pub("speed", &speed_msg);
Encoder left_encoder(19, 18);
Encoder right_encoder(21, 20);

void setup() {
  // put your setup code here, to run once:
  nh.getHardware()->setBaud(115200); //It's important to set it BEFORE init Node
  nh.initNode();
  nh.subscribe(cmd_vel);
  nh.advertise(vel_pub);
  initMotor();
  StopBothMotor();

  PID_left_motor.SetSampleTime(95);
  PID_right_motor.SetSampleTime(95);
  PID_left_motor.SetOutputLimits(-max_speed, max_speed);
  PID_right_motor.SetOutputLimits(-max_speed, max_speed);
  PID_left_motor.SetMode(AUTOMATIC);
  PID_right_motor.SetMode(AUTOMATIC);

  //Serial2.begin(115200);
}

void loop() {
  
  nh.spinOnce();
  if(ros_connect_status != nh.connected()){
    if(nh.connected()){
      nh.loginfo("Connected");
    }
    else {
      nh.loginfo("Not connected");
      StopBothMotor();
    }
    ros_connect_status = nh.connected();
  }

  if(millis() - lastMillis2 > 10){
    if(nh.connected()){
      analogWrite(LED_BUILTIN, led_status_fade);
      led_status_fade += 1;
      if(led_status_fade > 200){
        led_status_fade = 0;
      }
    }
    else{ analogWrite(LED_BUILTIN, 0); }
    lastMillis2 = millis();
  }
  
  if(millis() - lastMillis >= LOOPTIME){
      // Debugging speed
      /* if(left_motor_sp >= 0.0){
      if(last_val_1 != left_motor_sp){
        // Serial2.println("left_motor_sp value is:");
        // Serial2.println(left_motor_sp);
        // nh.loginfo("Left motor setpoint is");
        // nh.loginfo(left_motor_sp);
        last_val_1 = left_motor_sp;
      }
    }
    else{
      if(last_val_2 != left_motor_sp){
        // Serial2.println("any other value:");
        // Serial2.println(left_motor_sp);
        last_val_2 = left_motor_sp;
      }
    } */
    
      
      lastMillis = millis();
      //Encoder code
      if(abs(left_encoder.read()) < 5){
        left_act_spd = 0;
      }
      else{
        left_act_spd = measureSpeed(left_encoder.read(), LOOPTIME);
      }
      if(abs(right_encoder.read()) < 5){
        right_act_spd = 0;
      }
      else{
        right_act_spd = measureSpeed(right_encoder.read(), LOOPTIME);
      }
      left_encoder.write(0);
      right_encoder.write(0);

      //PID code
      PID_left_motor.Compute();
      // compute PWM value for left motor. Check constant definition comments for more information.
      if(left_reverse == true){
        PWM_leftMotor = constrain(((left_motor_sp+sgn(left_motor_sp)*min_speed_reverse_cmd)/speed_to_pwm_ratio) + (left_corr_spd/speed_to_pwm_ratio), -255, 255);
      }
      else{PWM_leftMotor = constrain(((left_motor_sp+sgn(left_motor_sp)*min_speed_cmd)/speed_to_pwm_ratio) + (left_corr_spd/speed_to_pwm_ratio), -255, 255);}

      PID_right_motor.Compute();
      if(right_reverse == true){
        PWM_rightMotor = constrain(((right_motor_sp+sgn(right_motor_sp)*min_speed_reverse_cmd)/speed_to_pwm_ratio) + (right_corr_spd/speed_to_pwm_ratio), -255, 255);
      }
      else{ PWM_rightMotor = constrain(((right_motor_sp+sgn(right_motor_sp)*min_speed_cmd)/speed_to_pwm_ratio) + (right_corr_spd/speed_to_pwm_ratio), -255, 255); }

      //Motor code

      if(noVelData >= noVelDataMax)
      {
        StopBothMotor();
      }
      //for LEFT motor

      if(left_motor_sp == 0)
      {
        setMotorSpeed(LEFT, 0);
//        nh.loginfo("left motor not actuating");/
      }
      else {
        setMotorSpeed(LEFT, PWM_leftMotor);
//        nh.loginfo("left vel executed");/
      }

      //for RIGHT motor
      if(right_motor_sp == 0)
      {
        setMotorSpeed(RIGHT, 0);
//        nh.loginfo("right motor not actuating");/
      }
      else {
        setMotorSpeed(RIGHT, PWM_rightMotor);
//        nh.loginfo("right vel executed");/
      }
      //Watchdog code
      noVelData++;
      if(noVelData >= 2000){
        noVelData = noVelDataMax;
      }

      if(millis() - lastMillis >= LOOPTIME){
        nh.loginfo("TOO LONG");
      }
      publishSpeed(LOOPTIME);
      }
    }

void setMotorSpeed(int i, int spd){
if(i == LEFT){
  if(spd < 0){
      spd = -spd;
      digitalWrite(DIR1_LEFT, HIGH);
      digitalWrite(DIR2_LEFT, LOW);
      analogWrite(PWM_LEFT, spd);
    }
    else{
      digitalWrite(DIR1_LEFT, LOW);
      digitalWrite(DIR2_LEFT, HIGH);
      analogWrite(PWM_LEFT, spd);
  }
  if(spd = 0){
    digitalWrite(DIR1_LEFT, LOW);
    digitalWrite(DIR2_LEFT, LOW);
    analogWrite(PWM_LEFT, brake_power);
    nh.loginfo("Left motor stopped");
  }
}
else{
  if(spd < 0){
    spd = -spd;
    digitalWrite(DIR1_RIGHT, HIGH);
    digitalWrite(DIR2_RIGHT, LOW);
    analogWrite(PWM_RIGHT, spd);
  }
  else{
    digitalWrite(DIR1_RIGHT, LOW);
    digitalWrite(DIR2_RIGHT, HIGH);
    analogWrite(PWM_RIGHT, spd);
  }
  if(spd = 0){
    digitalWrite(DIR1_RIGHT, LOW);
    digitalWrite(DIR2_RIGHT, LOW);
    analogWrite(PWM_RIGHT, brake_power);
    nh.loginfo("Right motor stopped");
  }
}
}

void initMotor(){
///  pinMode(EN1_LEFT, OUTPUT);
///  pinMode(EN2_LEFT, OUTPUT);
///  pinMode(EN1_RIGHT, OUTPUT);
///  pinMode(EN2_RIGHT, OUTPUT);
  pinMode(DIR1_LEFT, OUTPUT);
  pinMode(DIR2_LEFT, OUTPUT);
  pinMode(DIR1_RIGHT, OUTPUT);
  pinMode(DIR2_RIGHT, OUTPUT);
  pinMode(PWM_LEFT, OUTPUT);
  pinMode(PWM_RIGHT, OUTPUT);
  //Set enable pins high
//  digitalWrite(EN1_LEFT, HIGH);
//  digitalWrite(EN2_LEFT, HIGH);
//  digitalWrite(EN1_RIGHT, HIGH);
//  digitalWrite(EN2_RIGHT, HIGH);
  //Set motor off first
  analogWrite(PWM_LEFT, 0);
  analogWrite(PWM_RIGHT, 0);
}

void StopBothMotor(){
  digitalWrite(DIR1_LEFT, HIGH);
  digitalWrite(DIR2_LEFT, HIGH);
  digitalWrite(DIR1_RIGHT, HIGH);
  digitalWrite(DIR2_RIGHT, HIGH);
  analogWrite(PWM_LEFT, 0);
  analogWrite(PWM_RIGHT, 0);
}

//int PWM_compute()

float measureSpeed(long count, long time1){ //time in second, not millisecond
float speed1 = ((count/encoder_cpr)/wheel_gear_ratio) * PI  * (1000 / time1) * wheel_dia;
return speed1;
}

void publishSpeed(double time) {
  speed_msg.header.stamp = nh.now();      //timestamp for odometry data
  speed_msg.vector.x = left_act_spd;    //left wheel speed (in m/s)
  speed_msg.vector.y = right_act_spd;   //right wheel speed (in m/s)
  speed_msg.vector.z = time/1000;         //looptime, should be the same as specified in LOOPTIME (in s)
  vel_pub.publish(&speed_msg);
  nh.spinOnce();
//  nh.loginfo("Publishing odometry");/
}


template <typename T> int sgn(T val) {
return (T(0) < val) - (val < T(0));
}
