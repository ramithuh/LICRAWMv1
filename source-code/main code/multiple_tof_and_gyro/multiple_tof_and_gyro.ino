#include <Wire.h>
#include <VL53L0X.h>
#include <MPU6050_tockn.h>
/* IMPORTANT - YOU NEED TO CHANGE THE MPU6050_tockn.cpp library file
 change "done!!!" to "done!" */


 /*  SLAVEHC05-DevB:
              Address: 00-18-E4-34-CE-7A
              RFCOMM Channel: 1
              Requires Authentication: No
 */
MPU6050 mpu6050(Wire);

#define XSHUT_pin1 45           //Sensor #1
#define XSHUT_pin2 47           //Sensor #2 
#define XSHUT_pin3 49           //Sensor #3        
#define XSHUT_pin4 51           //Sensor #4
#define XSHUT_pin5 53           //Sensor #5

#define Sensor1_newAddress 42
#define Sensor2_newAddress 43
#define Sensor3_newAddress 44
#define Sensor4_newAddress 45
#define Sensor5_newAddress 46

VL53L0X Sensor1;
VL53L0X Sensor2;
VL53L0X Sensor3;
VL53L0X Sensor4;
VL53L0X Sensor5;

#define PINtoRESET 33


void setup()
{ /*WARNING*/
  pinMode(PINtoRESET, INPUT);    // Just to be clear, as default is INPUT. Not really needed.
  digitalWrite(PINtoRESET, LOW);
  
  //Shutdown pins of VL53L0X ACTIVE-LOW-ONLY NO TOLERANT TO 5V will fry them
  pinMode(XSHUT_pin1, OUTPUT);
  pinMode(XSHUT_pin2, OUTPUT);
  pinMode(XSHUT_pin3, OUTPUT);
  pinMode(XSHUT_pin4, OUTPUT);
  pinMode(XSHUT_pin5, OUTPUT);

  Serial.begin(9600);
  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);

  pinMode(XSHUT_pin5,INPUT);
  delay(10);
  Sensor5.setAddress(Sensor5_newAddress);
 
  pinMode(XSHUT_pin4,INPUT);
  delay(10);
  Sensor4.setAddress(Sensor4_newAddress);
  
  pinMode(XSHUT_pin3,INPUT);
  delay(10);
  Sensor3.setAddress(Sensor3_newAddress);
  
  pinMode(XSHUT_pin2,INPUT);
  delay(10);
  Sensor2.setAddress(Sensor2_newAddress);
  
  pinMode(XSHUT_pin1, INPUT);
  delay(10);
  Sensor1.setAddress(Sensor1_newAddress);
  


  Sensor1.init();
  Serial.print("Sensor 1 ok");
  Sensor2.init();
  Serial.print("Sensor 2 ok");
  Sensor3.init();
  Serial.print("Sensor 3 ok");
  Sensor4.init();
  Serial.print("Sensor 4 ok");
  Sensor5.init();
  Serial.print("Sensor 5 ok");
  

  Sensor1.setTimeout(500);
  Sensor2.setTimeout(500);
  Sensor3.setTimeout(500);
  Sensor4.setTimeout(500);
  Sensor5.setTimeout(500);
 

  Sensor1.startContinuous();
  Sensor2.startContinuous();
  Sensor3.startContinuous();
  Sensor4.startContinuous();
  Sensor5.startContinuous();
  

}



void print_tof_reading(int x){
  Serial.print(Sensor1.readRangeContinuousMillimeters());
  Serial.print(",  ");
  Serial.print(Sensor2.readRangeContinuousMillimeters());
  Serial.print(",  ");
  Serial.print(Sensor3.readRangeContinuousMillimeters());
  Serial.print(",  ");
  Serial.print(Sensor4.readRangeContinuousMillimeters());
  Serial.print(",  ");
  Serial.print(Sensor5.readRangeContinuousMillimeters());
  Serial.print("   ||  ");



  
  Serial.print("angleX : ");
  Serial.print(mpu6050.getAngleX());
  Serial.print(" angleY : ");
  Serial.print(mpu6050.getAngleY());
  Serial.print(" angleZ : ");
  Serial.println(mpu6050.getAngleZ());
  
  delay(x);
  
}



void loop() {
   //code used to read 'R' from serial and reset the robot.
  if (Serial.available() > 0) {
    if (Serial.read() == 'R') {
      Serial.println("========================================");
      Serial.println("       X      Restarting     X          ");
      Serial.println("========================================");
      delay(1000);
      pinMode(PINtoRESET,OUTPUT);

    }
  }

  
  mpu6050.update(); /* gyro */
  print_tof_reading(50);


}








