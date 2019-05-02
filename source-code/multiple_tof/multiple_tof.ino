#include <Wire.h>
#include <VL53L0X.h>
#include <AFMotor.h>

AF_DCMotor motor1(1);

#define XSHUT_pin2 53           //Sensor #2's XSHUT to Digital 5
#define Sensor2_newAddress 42

VL53L0X Sensor1;
VL53L0X Sensor2;

void setup()
{ /*WARNING*/
  //Shutdown pins of VL53L0X ACTIVE-LOW-ONLY NO TOLERANT TO 5V will fry them
  pinMode(XSHUT_pin2, OUTPUT);

  Serial.begin(9600);
  Wire.begin();
 
  Sensor2.setAddress(Sensor2_newAddress);
  pinMode(XSHUT_pin2, INPUT);
  delay(10);


  Sensor1.init();
  Sensor2.init();

  Sensor1.setTimeout(500);
  Sensor2.setTimeout(500);

  Sensor1.startContinuous();
  Sensor2.startContinuous();


    motor1.setSpeed(255);
}



void print_tof_reading(int x){
  Serial.print(Sensor1.readRangeContinuousMillimeters());
  Serial.print(',');
  Serial.print(Sensor2.readRangeContinuousMillimeters());
  Serial.println(' ');
  

if(Sensor1.readRangeContinuousMillimeters()>6000){
    motor1.setSpeed(255);
  motor1.run(FORWARD);
}else{
    motor1.setSpeed(0);
}
  
  delay(x);
  
}

void loop() 
{
 print_tof_reading(150);


//motor1.run(FORWARD);


}








