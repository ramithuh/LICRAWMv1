#include "libraries/LICRAWM-io.h"
#include <Wire.h>
#include <VL53L0X.h>

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

#include "libraries/LICRAWM-functions.h"
#include "libraries/LICRAWM-boot.h"

void setup() {
  boot();
  Serial2.begin(9600);
  Wire.begin();

  boot_tof();

  LED1.on(); //booting tof done!

}

void loop() {
 
  print_tof_reading();

}