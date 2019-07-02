#include "libraries/LICRAWM-io.h"
#include <Wire.h>
#include <VL53L0X.h>
#include <MPU6050_tockn.h>
#include <DualVNH5019MotorShield.h>
#include <QTRSensors.h>
#include <Servo.h>  
#include<Gaussian.h>
#include <LinkedList.h>
#include <GaussianAverage.h>
//#include<libraries/gyro.h>
/*******/ //experimental

GaussianAverage S_TOF4 = GaussianAverage(5);
GaussianAverage S_TOF5 = GaussianAverage(5);
GaussianAverage S_TOF3 = GaussianAverage(5);
GaussianAverage S_TOF2 = GaussianAverage(5);
GaussianAverage S_TOF1 = GaussianAverage(5);

float offset_TOF1=0;
float offset_TOF2=-26;
float offset_TOF3=-36.6;
float offset_TOF4=-17.8;
float offset_TOF5=-10.3;

/*******/ //experimental


_led LED5(LED_5);
_led LED1(LED_1);
_led LED3(LED_3);
_led LED2(LED_2);
_led LED4(LED_4);
_switch S2(S_2);
_switch S3(S_3);
_switch S4(S_4);

VL53L0X Sensor1;
VL53L0X Sensor2;
VL53L0X Sensor3;
VL53L0X Sensor4;
VL53L0X Sensor5;
QTRSensors linearray;
const uint8_t SensorCount = 15;
uint16_t sensorValues[SensorCount];

MPU6050 mpu6050(Wire, 0.1, 0.9);

DualVNH5019MotorShield md(39,38,5,40,A1,35,36,4,37,A0); ///remove current sense pins in future!
Servo pivot_servo;
Servo tilt_servo;
Servo grip_servo;
Servo coin_servo;

double angle_temp;  ///** experimental
unsigned long temp; ///** experimental


unsigned long O_Serial=micros();


String out;
#include "libraries/LICRAWM-functions.h"
#include "libraries/LICRAWM-boot.h"

float calculate_pos(){
  int pos=0;
  int on_count=0;
  for(int i=0;i<15;i++){
      if(sensorValues[i]>300){
          pos+=(i-7)*10;
          on_count+=1;
      }
  }
  /*
  if(on_count>9){
    if (sensorValues[0]<300 && sensorValues[14]>300){
      delay(200);
      md.setBrakes(400, 400); 
       md.setM1Speed(170);
       md.setM2Speed(-170);
       delay(350);
       md.setBrakes(400, 400); 
       delay(1000);
    }
    else if (sensorValues[0]>300 && sensorValues[14]<300){
      delay(200);
      md.setBrakes(400, 400); 
       md.setM1Speed(-170);
       md.setM2Speed(170);
       delay(350);
       md.setBrakes(400, 400); 
       delay(1000);
    }
  }*/
  if(on_count==0){
    return _last_position;
  }
  _last_position=pos/on_count;
  return _last_position;
  
}

void setup() {
  linearray.setTypeAnalog();
  linearray.setSensorPins((const uint8_t[]){A8, A1, A9, A2, A10, A3, A11, A4, A12, A5, A13, A6, A14, A7, A15}, SensorCount);
  linearray.setEmitterPin(LINE_ARRAY_EVEN_EMITTER_PIN);
  linearray.setEmitterPin(LINE_ARRAY_ODD_EMITTER_PIN);

  Serial2.begin(230400);
  //Serial2.setTimeout(100);
  Serial3.begin(9600);
  boot();
  Wire.begin();
  LED1.on();

  boot_gyro();
  delay(1000);

  boot_tof();
  LED5.on(); //booting tof done

  boot_motors();
  boot_encoders();
  boot_linearray();

  
  
  //setting up servos
  pivot_servo.attach(pivot_servo_pin);
  tilt_servo.attach(tilt_servo_pin);
  grip_servo.attach(gripper_servo_pin);
  coin_servo.attach(coin_servo_pin);

  //setting initial positions of servos


  //coin_servo_pos(1500); //starting postition of coin servo
  temp = micros(); //to make random turns

  delay(2000);
}


void loop(){


   //make_90_degree_clockwise();
   //delay(10000);

  out="";
  _input_check();   ///takes 4us
  //openmv_digital_decode();
  //get_tof_reading();
    
  

    if(FOLLOW_LINE){

        md.setM1Speed(m1_global_speed);
        md.setM2Speed(m2_global_speed);
        unsigned int sensors[no_of_sensors];
        //reading the white line
      //  linearray.readLineWhite(sensorValues);
       linearray.readLineBlack(sensorValues);
        int position=calculate_pos();

        out+=":POS:";
        out+=position;

        Serial2.print("Pos:");
        Serial2.print(position);


        //calculating Error
        int error = position ;
        Serial2.print(" :Error:");
        Serial2.print(error);


        //PID controlling
        float motor_speed = KP*error + KI*(error + last_error) + KD*(error - last_error);
        Serial2.print(" :motor_speed_before_mapping:");
        Serial2.print(motor_speed);


        //mapping the motor speed to the range of the motors
        //motor_speed = map(motor_speed,KP*-7000,KP*7000,-15,15);
        Serial2.print(" :motor_speed_mapped:");
        Serial2.print(motor_speed);


        last_error = error;

        m1_global_speed = m1_global_speed - motor_speed;    //plus or minus may change accordingly
        m2_global_speed = m2_global_speed + motor_speed;

        Serial2.print(" :::M1speed=");
        Serial2.print(m1_global_speed);
        Serial2.print(" ::M2speed=");
        Serial2.println(m2_global_speed);
        if (m1_global_speed>400){
          m1_global_speed = 400;
        }
        else if (m1_global_speed<0){
          m1_global_speed = 0;
        }
        if (m2_global_speed>400){
          m2_global_speed = 400;
        }
        else if (m2_global_speed<0){
          m2_global_speed = 0;
        }
    }
/* 
    if(0){
      if(Sensor1.readRangeContinuousMillimeters()>150){
        move_fixed_distance(1000);
        delay(2000);
         //move_fixed_distance(1000);
        //md.setM1Speed(m1_global_speed);
        //md.setM2Speed(m2_global_speed);
      }else{
         md.setBrakes(400, 400);
         //delay(500);
         //make_90_degree_clockwise();
         
      }
    }


*/
/*
  if(FOLLOW_WALL){
        md.setM1Speed(m1_global_speed);
        md.setM2Speed(m2_global_speed);

        float tof1 = Sensor1.readRangeContinuousMillimeters();
        //float tof2 = Sensor2.readRangeContinuousMillimeters();
        //float tof3 = Sensor3.readRangeContinuousMillimeters();
        float tof4 = Sensor4.readRangeContinuousMillimeters();
        float tof5 = Sensor5.readRangeContinuousMillimeters();
        
        out+="T1:";
        out+=tof1;
        out+=":T2:";
        out+='0';
        out+=":T3:";
        out+='0';
        out+=":T4:";
        out+=tof4;
        out+=":T5:";
        out+=tof5;
        
        if (tof1<=150){
          md.setBrakes(400, 400); 
          delay(1000);
          md.setM1Speed(-170);
          md.setM2Speed(170);
          delay(500);
          md.setBrakes(400, 400); 
          
          
        }else{
          tof_error = calculate_tof_error(tof5,tof4);

          m1_global_speed = m1_global_speed - tof_error;
          m2_global_speed = m2_global_speed + tof_error;

          if (m1_global_speed>400){
            m1_global_speed = 400;
          }
          else if (m1_global_speed<150){
            m1_global_speed = 150;
          }
          if (m2_global_speed>400){
            m2_global_speed = 400;
          }
          else if (m2_global_speed<150){
            m2_global_speed = 150;
          }
        }
    }

  */
/*
  get_gyro_reading();
  get_encoder_reading();
  get_line_array();
  int position=calculate_pos();
  out+=":POS:";
  out+=position;

  if(VISUALIZE && (micros()-O_Serial)>WRITE_EVERY_MS){
    Serial2.println(out);
    O_Serial=micros();
  } */

}