/* This example shows how to use continuous mode to take
range measurements with the VL53L0X. It is based on
vl53l0x_ContinuousRanging_Example.c from the VL53L0X API.

The range readings are in units of mm. */

#include <Wire.h>
#include <VL53L0X.h>
#include <DualVNH5019MotorShield.h>




DualVNH5019MotorShield md(35,36,4,37,A0,38,39,5,40,A1) ;
VL53L0X sensor;

void setup()
{
  Serial.begin(9600);
  Wire.begin();

  sensor.init();
  sensor.setTimeout(500);

  // Start continuous back-to-back mode (take readings as
  // fast as possible).  To use continuous timed mode
  // instead, provide a desired inter-measurement period in
  // ms (e.g. sensor.startContinuous(100)).
  sensor.startContinuous();
}

void loop()
{
  int dis=sensor.readRangeContinuousMillimeters();
  Serial.print(dis);
  //if (sensor.timeoutOccurred()) { Serial.print(" TIMEOUT"); }
 
  
  Serial.println();
  if (dis<50){
  //   md.setM1Speed(0);
  //md.setM2Speed(0);
    md.setM1Brake(400);
    
    md.setM2Brake(400);
    
    //delay(500);
  }else{

     md.setM1Speed(80);
  md.setM2Speed(100);
  }

  //delay(100);
}
