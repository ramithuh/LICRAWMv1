
#include "libraries/LICRAWM-io.h"
#include <Wire.h>
#include <VL53L0X.h>
#include <MPU6050_tockn.h>
#include <DualVNH5019MotorShield.h>
<<<<<<< HEAD
#include <Servo.h>  
#include<Gaussian.h>
#include <LinkedList.h>
#include <GaussianAverage.h>

/*******/ //experimental

GaussianAverage S_TOF4 = GaussianAverage(10);
GaussianAverage S_TOF5 = GaussianAverage(10);
GaussianAverage S_TOF3 = GaussianAverage(10);
GaussianAverage S_TOF2 = GaussianAverage(10);

float offset_TOF4=-17.8;
float offset_TOF5=-10.3;

float offset_TOF2=-26;
float offset_TOF3=-36.6;
/*******/ //experimental
=======
#include <Servo.h>
#include <QTRSensors.h>
#include <DualVNH5019MotorShield.h>
>>>>>>> 11b551fe288bea262535d23004f3c714fbba6922

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
Servo pivot_servo;
Servo tilt_servo;
Servo grip_servo;
Servo coin_servo;

DualVNH5019MotorShield md(38,39,5,40,A1,35,36,4,37,A0);

//analog pin numbers used inside the curly brackets
QTRSensorsA qtr((char[] {0,1,2},no_of_sensors);

unsigned long O_Serial=micros();
int last_error = 0;
int motor_speed = 0;


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
  //setting up servos
  pivot_servo.attach(pivot_servo_pin);
  tilt_servo.attach(tilt_servo_pin);
  grip_servo.attach(gripper_servo_pin);
  coin_servo.attach(coin_servo_pin);

  //setting initial positions of servos
  arm_position(1000,1200,2400,1000);
  coin_servo.writeMicroseconds(0);        //change the duty cycle accordingly

  //calibrating the sensor array
  int i;
  for (i=0; i<250; i++){
    qtr.calibrate();
    delay(20);
  }

  //initializing the motor driver
  md.init();

}


void loop(){

  out="";
  //md.setM1Speed(m1_global_speed);
  //md.setM2Speed(m2_global_speed);
  unsigned long start = micros();
  

  _input_check();   ///takes 4us
  
  get_tof_reading();
  get_gyro_reading();
  get_encoder_reading();


  unsigned long end = micros();
  unsigned long delta = end - start;

 // Serial2.println(delta);

 if(DEBUG_SPEED){
    Serial2.print(F("|-> Loop ran in "));
    Serial2.print(delta);
    Serial2.print("us && Free SRAM:");
    Serial2.println(freeRam());
  }
  
  if(VISUALIZE && (micros()-O_Serial)>WRITE_EVERY_MS){
    Serial2.println(out);
    O_Serial=micros();
  }

  while (FOLLOW_LINE)
  {
    md.setM1Speed(left_motor);
    md.setM2Speed(right_motor);

    unsigned int sensors[no_of_sensors];
    //reading the white line
    int position = qtr.readLine(sensors,QTR_EMITTERS_ON,1);
    //calculating Error
    int error = position - mid_val;
    //PID controlling
    int motor_speed = KP*error + KI*(error + last_error) + KD*(error - last_error);
    //mapping the motor speed to the range of the motors
    motor_speed = map(motor_speed,0,1000*(no_of_sensors-1),-400,400);
    last_error = error;

    left_motor = left_motor - motor_speed;    //plus or minus may change accordingly
    right_motor = right_motor + motor_speed;
  }
  

}