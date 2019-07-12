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
const uint8_t SensorCount = 15;
uint16_t sensorValues[SensorCount];

//MPU6050 mpu6050(Wire, 0.1, 0.9);

DualVNH5019MotorShield md(39,38,5,40,A1,35,36,4,37,A0); ///remove current sense pins in future!

Servo arm_servo;
Servo coin_servo;

unsigned long O_Serial=micros();


String out;
#include "libraries/LICRAWM-functions.h"
#include "libraries/LICRAWM-boot.h"


//float error_weights[]={-4.2875, -2.7, -1.5625, -0.8, -0.3375, -0.1, -0.0125, 0, 0.0125, 0.1, 0.3375, 0.8, 1.5625, 2.7, 4.2875};
int error_weights[]={-110, -90, -70, -50, -35, -20, -10, 0, 10, 20,  35, 50, 70, 90, 110};


float calculate_pos(int threshold = 300){
  linearray.readLineWhite(sensorValues);
  out="T1:0:T2:0:T3:0:T4:0:T5:0:Z:0:Y:0:X:0:M1:0:M2:0";

  out+=":L:";
  for (uint8_t i = 0; i < SensorCount; i++){
    out+=sensorValues[i];
    out+=":";
  }

 

  int pos=0;
  int on_count=0;
  for(int i=0;i<15;i++){
      if(sensorValues[i]<threshold){
          pos+=(i-7)*10;
         // pos+=error_weights[i];
          on_count+=1;
      }
  }

  out+=":POS:";
  out+=pos;
  out+=":T:";
  out+=threshold;

  Serial2.println(out);

  if(on_count>11){
    Serial2.print("On count: ");
    Serial2.println(on_count);
   
    md.setBrakes(300,300);
    move_fixed_distance(180);
    md.setBrakes(300,300);

     
    if (((digitalRead(LEFT_TRACKER)==0 || digitalRead(RIGHT_TRACKER)==0)) || on_count == 15){      //Color is in FOV, or ALL sensors on!
      md.setBrakes(300,300);
      flag = 1;
      flag_count += 1;
      if (flag_count==2 && on_count < 15){
        md.setBrakes(300,300);
        delay(1000);
        md.setM2Speed(default_m2_speed);
        md.setM1Speed(0);
        delay(110);
        md.setBrakes(300,300);
        delay(1000);
      }
      Serial2.print("| flag_count :");
      Serial2.print(flag_count);
      Serial2.print(" on_sensor_count: ");
      Serial2.print(on_count);
      Serial2.print(" threshold: ");
      Serial2.println(threshold);
      return 0;
    }
    if (sensorValues[0]<threshold && sensorValues[14]>threshold){
      //delay(200);
      //md.setBrakes(400, 400); 
      //delay(200);
      move_fixed_distance(700);
      delay(200);
      make_90_degree_anticlockwise(180,210);
      md.setBrakes(300, 300);
      delay(200);
    }
    else if (sensorValues[0]>threshold && sensorValues[14]<threshold){
      //delay(200);
      //md.setBrakes(400, 400); 
      //delay(200);
      move_fixed_distance(700);
      delay(200);
      make_90_degree_clockwise(180,210);
      md.setBrakes(300, 300);
      delay(500);
    }
  
  }
  if(on_count==0){
    return _last_position;
  }
  _last_position=pos/on_count;
  return _last_position;
  
}


void setup() {
  //setting up servos (arm and coin)
  arm_servo.attach(arm_servo_pin);
  coin_servo.attach(coin_servo_pin);

  //setting initial positions of servos
  coin_servo_pos(1500); 
  
  arm_servo.writeMicroseconds(2050);
  delay(1000);

  linearray.setTypeAnalog();
  linearray.setSensorPins((const uint8_t[]){A8, A1, A9, A2, A10, A3, A11, A4, A12, A5, A13, A6, A14, A7, A15}, SensorCount);
  linearray.setEmitterPin(LINE_ARRAY_EVEN_EMITTER_PIN);
  linearray.setEmitterPin(LINE_ARRAY_ODD_EMITTER_PIN);


  Serial2.begin(230400);
 
  boot();
  Wire.begin();
  LED1.on();

//  boot_gyro();
  delay(1000);

  boot_tof();
  LED5.on(); //booting tof done

  boot_motors();
  boot_encoders();
  boot_linearray();


  delay(3000);
}

void loop(){
 
  out="";
  
  _input_check();   ///takes 4us
  

  //get_tof_reading();
  if (openmv_digital_decode()==-1){
    _LED_all_on();
  }

  if(WATER_TRANSFER){
  //turn 180 degrees to transfer water from the back
  make_90_degree_clockwise();
  delay(1000);
  make_90_degree_clockwise();

  //move forward to align with the line

  //reverse to the containers
  move_fixed_distance(1600,-default_m1_speed,-default_m2_speed);
    //lowering the arm
    for (int j = 2000;j>=1475;j=j-25){
      arm_servo.writeMicroseconds(j);
      delay(100);
    }
    delay(1000);      //delay to transfer water
    digitalWrite(8,HIGH);//start water transfer
    delay(20000);
    digitalWrite(8,LOW);//finished transfering water

    //lifting the arm
    for (int i = 1475;i<2000;i=i+25){
      arm_servo.writeMicroseconds(i);
      delay(100);
    }
      delay(1000);    //wait till water is drained
  }
    
  if(FOLLOW_LINE){
        //noInterrupts();
        md.setM1Speed(m1_global_speed);
        md.setM2Speed(m2_global_speed);
        unsigned int sensors[no_of_sensors];
        //reading the white line
       
       int position;

       if(flag_count==2 || flag_count==3 || flag_count==4){
          position=calculate_pos(700);
       }else{
          position=calculate_pos();
       }

       if (flag == 1 && (flag_count == 1|| flag_count ==6)){//start and end of arena
          Serial2.println("| Arena Start/END");
          move_fixed_distance(2000);
          flag=0;
          position=calculate_pos();
          

        }else if (flag == 1 && flag_count==2){   //coin pick
          md.setBrakes(300,300);
          Serial2.println("| Going to Pick Coin");
          Serial2.println("|   Reversed 4.5cm");
          move_fixed_distance(700,-default_m1_speed,-default_m2_speed);//4.5 cm reverse 

          Serial2.println("|   Reading Color for 15s....");
          coin_colour = read_colour();

          Serial2.print("|   Color is ");
          Serial2.println(coin_colour);

          Serial2.println("|   Moving forward to pick!");
         
          move_fixed_distance(1000,180,180);
          delay(1000);
          move_fixed_distance(700);
          delay(1000);
          flag=0;
          position = calculate_pos();
         
        }else if (flag==1 && flag_count==3){
          if(coin_colour == 0){ //RED COLOR
            move_fixed_distance(400);
            make_45_degree_clockwise();
            //move_fixed_distance(900);
          }else if (coin_colour == 2){ //BLUE COLOR
            move_fixed_distance(500);
            make_45_degree_anticlockwise();
            //move_fixed_distance(900);
          } else{//GREEN COLOR (go straight) 
            move_fixed_distance(500);
          }
          delay(1000);
          flag=0;
          position = calculate_pos(700);
        }
        else if (flag==1 && flag_count==5){
          md.setBrakes(300,300);
          Serial2.println("|Going to place the coin!");
          move_fixed_distance(500);
          coin_place();
          Serial2.println("|Coin placed!");
          move_fixed_distance(500);
          coin_pick();
          flag=0;
          position = calculate_pos(700); //follow the rest of the color line
        }
      
       // Serial2.print("Pos:");
      //  Serial2.print(position);


        //calculating Error
        int error = position ;
       // Serial2.print(" :Error:");
        //Serial2.print(error);


        //PID controlling
        float motor_speed = KP*error + KI*(error + last_error) + KD*(error - last_error);
       // Serial2.print(" :motor_speed_before_mapping:");
       // Serial2.print(motor_speed);

        last_error = error;

        m1_global_speed = m1_global_speed - motor_speed;    //plus or minus may change accordingly
        m2_global_speed = m2_global_speed + motor_speed;

        //Serial2.print(" :::M1speed=");
        //Serial2.print(m1_global_speed);
        //Serial2.print(" ::M2speed=");
        //Serial2.println(m2_global_speed);
        if (m1_global_speed>230){
          m1_global_speed = 230;
        }
        else if (m1_global_speed<30){
          m1_global_speed = 30;
        }
        if (m2_global_speed>230){
          m2_global_speed = 230;
        }
        else if (m2_global_speed<30){
          m2_global_speed = 30;
        }
      
    }

  if(FOLLOW_WALL){

      //  md.setM1Speed(m1_wall_follow);
        //md.setM2Speed(m2_wall_follow);

        float tof1 = Sensor1.readRangeContinuousMillimeters()+offset_TOF1;
        float tof2 = Sensor2.readRangeContinuousMillimeters()+offset_TOF2;
        float tof3 = Sensor3.readRangeContinuousMillimeters()+offset_TOF3;
        float tof4 = Sensor4.readRangeContinuousMillimeters()+offset_TOF4;
        float tof5 = Sensor5.readRangeContinuousMillimeters()+offset_TOF5;
        
        out+="T1:";
        out+=tof1;
        out+=":T2:";
        out+='0';
        out+=":T3:";
        out+=tof3;
        out+=":T4:";
        out+=tof4;
        out+=":T5:";
        out+=tof5;
        
        if (tof4>150 && tof5>150){  // can I turn Left ??

                Serial2.println("#no wall in left");
                md.setBrakes(400,400);

                LED3.on();
                make_90_degree_anticlockwise();
                LED3.off();
                
                delay(1000);  
                move_fixed_distance_with_walls(2500);
                delay(1000);
              
        }else if (tof1>150){
                Serial2.println("#wall in left & NO wall in FRONT");

                _LED_all_off();
                md.setBrakes(400,400);
                move_fixed_distance_with_walls(2500);
              
                delay(1000);

        }else if (tof3>150){
                Serial2.println("#wall in left & front NOT in RIGHT");
                md.setBrakes(400,400);

                LED5.on();
                make_90_degree_clockwise();
                LED5.off();

                delay(1000);
                move_fixed_distance_with_walls(2500);   
                delay(1000);

        }else{
                md.setBrakes(300,300);
                LED2.on();
                make_90_degree_clockwise();
                delay(1000);
                make_90_degree_clockwise();
                delay(1000);
                LED2.on();

                move_fixed_distance_with_walls(2500);
                
                delay(1000);
        }
    }


/**/
   // get_gyro_reading();

 /*        
    get_encoder_reading();
    get_line_array();
 //   int position=calculate_pos();
   // out+=":POS:";
   // out+=position; */
 
  if(VISUALIZE && (micros()-O_Serial)>WRITE_EVERY_MS){
    Serial2.println(out);
    O_Serial=micros();
  } 

}