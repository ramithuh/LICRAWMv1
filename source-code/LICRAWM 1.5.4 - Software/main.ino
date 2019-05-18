
#include "libraries/LICRAWM-io.h"
#include <Wire.h>
#include <VL53L0X.h>
#include <MPU6050_tockn.h>
#include <DualVNH5019MotorShield.h>

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

DualVNH5019MotorShield md(38,39,5,40,A1,35,36,4,37,A0); ///remove current sense pins in future!!

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

  boot_motors();
  boot_encoders();

}


void loop(){

  out="";
  md.setM1Speed(m1_global_speed);
  md.setM2Speed(m2_global_speed);
  unsigned long start = micros();
  

  _input_check();  
  
  get_tof_reading();
  get_gyro_reading();
  get_encoder_reading();


  unsigned long end = micros();
  unsigned long delta = end - start;

  if(DEBUG_SPEED){
    Serial2.print("-> Loop ran in ");
    Serial2.print(delta);
    Serial2.println("ns");
  }
  
  if(VISUALIZE && (micros()-O_Serial)/1000>WRITE_EVERY_MS){
    Serial2.println(out);
    O_Serial=micros();
  }

}