
#include "libraries/LICRAWM-io.h"
#include <Wire.h>
#include <VL53L0X.h>
#include <MPU6050_tockn.h>


_led LED5(LED_5);
_led LED1(LED_1);
_led LED3(LED_3);
_led LED2(LED_2);
_led LED4(LED_4);
_switch S2(S_2);
_switch S3(S_3);
_switch S4(S_4);

VL53L0X Sensor3;
VL53L0X Sensor4;
MPU6050 mpu6050(Wire);

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

}

void loop(){

  unsigned long start = micros();

    
    //TraceFunc();
    bluetooth_input_check(); 
    //openmv_digital_decode();
  
    get_tof_reading();
   
    get_gyro_reading();

  unsigned long end = micros();
  unsigned long delta = end - start;

  if(DEBUG_SPEED){
    Serial2.print("\n -> Loop ran in ");
    Serial2.print(delta);
    Serial2.print("ns \n");
  }

}