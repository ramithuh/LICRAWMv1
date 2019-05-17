
#include "libraries/LICRAWM-io.h"
#include <Wire.h>
#include <VL53L0X.h>
#include <MPU6050_tockn.h>
#include <Servo.h>


_led LED5(LED_5);
_led LED1(LED_1);
_led LED3(LED_3);
_led LED2(LED_2);
_led LED4(LED_4);
_switch S2(S_2);
_switch S3(S_3);
_switch S4(S_4);

VL53L0X Sensor2;
VL53L0X Sensor3;
VL53L0X Sensor4;
VL53L0X Sensor5;
MPU6050 mpu6050(Wire);

Servo pivot_servo;
Servo tilt_servo;
Servo grip_servo;
Servo coin_servo;

unsigned long O_Serial=micros();

String out;
#include "libraries/LICRAWM-functions.h"
#include "libraries/LICRAWM-boot.h"


void setup() {


  Serial2.begin(230400);
  //Serial2.setTimeout(100);
  Serial3.begin(9600);
  boot();
  Wire.begin();
  LED1.on();

  boot_gyro();
  LED3.on(); //booting gyro done!
  boot_tof();
  LED5.on(); //booting tof done!

  //setting up servos
  pivot_servo.attach(pivot_servo_pin);
  tilt_servo.attach(tilt_servo_pin);
  grip_servo.attach(gripper_servo_pin);
  coin_servo.attach(coin_servo_pin);

  //setting initial positions of servos
  arm_position(1000,1200,2400,1000);
  coin_servo.writeMicroseconds(0);        //change the duty cycle accordingly

}

void loop(){

  out="";

  unsigned long start = micros();
  
    //TraceFunc();
  _input_check(); 
    //openmv_digital_decode();
  
  get_tof_reading();
   
  get_gyro_reading();

  unsigned long end = micros();
  unsigned long delta = end - start;

  if(DEBUG_SPEED){
    Serial2.print("\n -> Loop ran in ");
    Serial2.print(delta);
    Serial2.print("ns \n");
  }else if(!DEBUG_GYRO && !DEBUG_TOF && (micros()-O_Serial)/1000>WRITE_EVERY_MS){
    Serial2.println(out);
    O_Serial=micros();
  }

}