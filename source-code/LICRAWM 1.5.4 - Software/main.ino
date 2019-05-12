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
  Serial2.begin(9600);
  boot();
  Wire.begin();
  LED2.on();

  boot_gyro();
  LED4.on(); //booting gyro done!
  boot_tof();
  LED1.on(); //booting tof done!

}

void loop(){

  serial_input_check(); 
 
  print_tof_reading();

  mpu6050.update(); 
  print_gyro_reading();

}