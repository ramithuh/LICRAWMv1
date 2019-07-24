#include<util/atomic.h>
#include <Wire.h>
#include "libraries/gyro.h"
#include "libraries/LICRAWM-io.h"
#include <VL53L0X.h>
#include <DualVNH5019MotorShield.h>
#include <QTRSensors.h>
#include <Servo.h>  
#include<Gaussian.h>
#include <LinkedList.h>
#include <GaussianAverage.h>
/*******/ //experimental

GaussianAverage S_TOF4 = GaussianAverage(5);
GaussianAverage S_TOF5 = GaussianAverage(5);
GaussianAverage S_TOF3 = GaussianAverage(5);
GaussianAverage S_TOF2 = GaussianAverage(5);
GaussianAverage S_TOF1 = GaussianAverage(5);

float offset_TOF1=-50;//0;
float offset_TOF2=-5;//-17;
float offset_TOF3=-29;//-34;//-10.3;
float offset_TOF4=-18;//-18.8;
float offset_TOF5=-36;//-29.6;//

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
const uint8_t SensorCount = 16;
uint16_t sensorValues[SensorCount];

DualVNH5019MotorShield md(39,38,5,40,A1,35,36,4,37,A0); ///remove current sense pins in future!

Servo arm_servo;
Servo coin_servo;

unsigned long O_Serial=micros();

int error_weights[]={-160, -140, -120, -100, -80, -60, -25, -20, 20, 25,  60, 80, 100, 120, 140, 160};

String out;
#include "libraries/LICRAWM-functions.h"
#include "libraries/LICRAWM-boot.h"
#include "libraries/LICRAWM-tasks.h"



void setup() {
 
  arm_servo.attach(arm_servo_pin);     //setting up servos (arm and coin)
  coin_servo.attach(coin_servo_pin);

  //coin_place();
  coin_pick();  //coin pick position to the servo
  
  arm_servo.writeMicroseconds(2050);
  delay(1000);

  linearray.setTypeAnalog();
  linearray.setSensorPins((const uint8_t[]){A0, A8, A1, A9, A2, A10, A3, A11, A4, A12, A5, A13, A6, A14, A7, A15}, SensorCount);
  linearray.setEmitterPin(LINE_ARRAY_EVEN_EMITTER_PIN);
  linearray.setEmitterPin(LINE_ARRAY_ODD_EMITTER_PIN);

  Serial2.begin(230400); //bluetooth setup
 
  boot();
  Wire.begin();
  LED1.on();

  delay(1000);

  boot_tof();
  LED5.on(); //booting tof done
  

  boot_motors();
  boot_encoders();
  boot_linearray();
  delay(2000);

  //move_fixed_distance(2500);
  start_square();
  delay(2000);
  
}

void loop(){
  
  //_input_check();   ///takes 4us
 // if(flag_count==0){
line_follow();
 // }else{
   //  md.setBrakes(400,400);
  }

//align_left();
//line_follow();
//FOLLOW_WALL();
//out="";
//get_line_array();
//get_tof_reading();
//align_right();

/*
 
  line_follow();
  //picking up the coin
  coin_collect();
  line_follow(300,900);
  //line_follow(700);
  delay(1000);
  coin_colour=0;
  //color line following 
  if(coin_colour == 0){ //RED COLOR
    //move_fixed_distance(500);
    make_45_degree_clockwise();
    move_fixed_distance(500);
  }
  else if (coin_colour == 2){ //BLUE COLOR
    //move_fixed_distance(300);
    make_45_degree_anticlockwise();
    move_fixed_distance(500);
  } 
  else{//GREEN COLOR (go straight) 
    move_fixed_distance(950);
  }
  delay(1000);
  move_fixed_distance(200,-default_m2_speed,-default_m1_speed);
  line_follow(700,300);
  delay(1000);
  line_follow(700);
  delay(1000);
  line_follow(700);
  delay(5000);
/* 
  line_follow();
  line_follow();

  //coin place
  md.setBrakes(300,300);
  Serial2.println("|Going to place the coin!");
  move_fixed_distance(500);
  coin_place();
  Serial2.println("|Coin placed!");
  move_fixed_distance(500);
  coin_pick();

  line_follow();*/ 

  if(VISUALIZE && (micros()-O_Serial)>WRITE_EVERY_MS){
    Serial2.println(out);
    O_Serial=micros();
  } 

}

//Yayy done!!! :D