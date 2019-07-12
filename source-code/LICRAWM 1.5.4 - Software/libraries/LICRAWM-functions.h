/* Contains LICRAWM BOARD FUNCTIONS */

#define TraceFunc()   do { Serial2.print(F("In function: ")); Serial2.println(__func__); } while (0)


int freeRam() {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}


void _LED_all_off(int miliseconds=0){
    digitalWrite(LED_1, LOW);   
    digitalWrite(LED_2, LOW); 
    digitalWrite(LED_3, LOW); 
    digitalWrite(LED_4, LOW); 
    digitalWrite(LED_5, LOW); 
    delay(miliseconds);
}

void _LED_all_blink(int miliseconds=0){
    digitalWrite(LED_1, LOW);   
    digitalWrite(LED_2, LOW); 
    digitalWrite(LED_3, LOW); 
    digitalWrite(LED_4, LOW); 
    digitalWrite(LED_5, LOW); 
    delay(miliseconds);
    digitalWrite(LED_1, LOW);   
    digitalWrite(LED_2, LOW); 
    digitalWrite(LED_3, LOW); 
    digitalWrite(LED_4, LOW); 
    digitalWrite(LED_5, LOW); 
}

void _LED_all_on(int miliseconds=0){
    digitalWrite(LED_1, HIGH);   
    digitalWrite(LED_2, HIGH); 
    digitalWrite(LED_3, HIGH); 
    digitalWrite(LED_4, HIGH); 
    digitalWrite(LED_5, HIGH); 
    delay(miliseconds);
}


void get_tof_reading(int miliseconds=0){
   double r1=999;
   double r2=999;
   double r3=999;
   double r4=999;
   double r5=999;

  if(UPDATE_TOF==1){
    r1=Sensor1.readRangeContinuousMillimeters();
    r2=Sensor2.readRangeContinuousMillimeters();
    r3=Sensor3.readRangeContinuousMillimeters();  //this is faster than single read
    r4=Sensor4.readRangeContinuousMillimeters();
    r5=Sensor5.readRangeContinuousMillimeters();
  }

/*** smoothen  & calib **/
r1+=offset_TOF1;
r2+=offset_TOF2;
r3+=offset_TOF3;
r4+=offset_TOF4;
r5+=offset_TOF5;

S_TOF4+=r4;
S_TOF5+=r5;
S_TOF3+=r3;
S_TOF2+=r2;
S_TOF1+=r1;
r4=S_TOF4.process().mean;
r5=S_TOF5.process().mean;
r3=S_TOF3.process().mean;
r2=S_TOF2.process().mean;
r1=S_TOF1.process().mean;

//** **/
  if(DEBUG_TOF==true){
        Serial2.print(F("T1:"));
        Serial2.print(r1);
        if (r1>=65530){ Serial2.print(F(" Error: ToF1 -> TIMEOUT")); }

        Serial2.print(F("T2:"));
        Serial2.print(r2);
        if (r2>=65530){ Serial2.print(F(" Error: ToF2 -> TIMEOUT")); }

        Serial2.print(F(":T3:"));
        Serial2.print(r3);
        if (r3>=65530){ Serial2.print(F(" Error: ToF3 -> TIMEOUT")); }

        Serial2.print(F(":T4:"));
        Serial2.print(r4);
        if (r4>=65530) { Serial2.print(F(" Error: ToF4 -> TIMEOUT")); }

        Serial2.print(F(":T5:"));
        Serial2.print(r5);
        if (r5>=65530){ Serial2.print(F(" Error: ToF5 -> TIMEOUT")); }

        Serial2.print(F("\n"));

        delay(miliseconds);

  }else if(VISUALIZE){
        out+="T1:";
        out+=r1;
        out+=":T2:";
        out+=r2;
        out+=":T3:";
        out+=r3;
        out+=":T4:";
        out+=r4;
        out+=":T5:";
        out+=r5;
  }
  
}

void get_gyro_reading(int miliseconds=0){

    if(UPDATE_GYRO){
       /*readAndProcessAccelData();
       readAndProcessGyroData();
       calculateAngle();*/
       
        /* mpu6050.update(); 
      
        x=mpu6050.getAngleX();
        y=mpu6050.getAngleY();
        z=mpu6050.getAngleZ();
        x=mpu6050.getGyroAngleX();
        y=mpu6050.getGyroAngleY();
        z=mpu6050.getGyroAngleZ();*/

          readAndProcessGyroData();
          delay(50);
    }

    
    if(DEBUG_GYRO==true){      /* displays only if needed */
        Serial2.print(F("angleX:"));
        Serial2.print(angleX);
        Serial2.print(F(":Y:"));
        Serial2.print(angleY);
        Serial2.print(F(":Z:"));
        Serial2.println(angleZ);
        delay(miliseconds);
    }
    if(VISUALIZE){
        out+=":Z:";
        out+=angleZ;
        out+=":Y:";
        out+=angleY;
        out+=":X:";
        out+=angleX;
    
    }
}
void get_encoder_reading(){
  if(DEBUG_ENCODERS){
        Serial2.print("M1 Count: "); //(motor B)
        Serial2.print(M1count);
        Serial2.print(" M2 Count: ");
        Serial2.println(M2count);
  }

  if(VISUALIZE){
      out+=":M1:";
      out+=M1count;
      out+=":M2:";
      out+=M2count;

  }
}

void rightEncoderEvent() {
  if (digitalRead(M1_ENCODER_A) == HIGH) {
    if (digitalRead(M1_ENCODER_B) == LOW) {
      M1count++;
    } else {
      M1count--;
    }
  } else {
    if (digitalRead(M1_ENCODER_B) == LOW) {
      M1count--;
    } else {
      M1count++;
    }
  }
}
void leftEncoderEvent() {
  if (digitalRead(M2_ENCODER_A) == HIGH) {
    if (digitalRead(M2_ENCODER_B) == LOW) {
      M2count++;
    } else {
      M2count--;
    }
  } else {
    if (digitalRead(M2_ENCODER_B) == LOW) {
      M2count--;
    } else {
      M2count++;
    }
  }
}




void coin_servo_pos(int pulse=0){
  coin_servo.writeMicroseconds(pulse);
}

void get_line_array(){
  //uint16_t position = linearray.readLineBlack(sensorValues);
  uint16_t position = linearray.readLineWhite(sensorValues);


  out+=":L:";
  for (uint8_t i = 0; i < SensorCount; i++){
    out+=sensorValues[i];
    out+=":";
  }
}

void move_fixed_distance(int distance ,int speed1=default_m1_speed,int speed2=default_m2_speed){
  interrupts();
  int m1=0;
  int m2=0;

  ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
    M1count=0;
    M2count=0;
  }

  while(abs(m1)<distance || abs(m2)<distance){
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
        m1=M1count;
        m2=M2count;
    }

    if(abs(m1)>distance)md.setM1Brake(400);
    else md.setM1Speed(speed1);
    if(abs(m2)>distance)md.setM2Brake(400);
    else md.setM2Speed(speed2);
    
  }
  if (flag_count==1){
    md.setBrakes(300,300);
  }else{
    md.setBrakes(400,400);
  }
    //if above brakes fails
  //Serial2.println("|##-> moved fixed distance! ");

}

void move_fixed_distance_with_tof(int distance ,int speed1=default_m1_speed,int speed2=default_m2_speed){
  //move with constraint of TOF front!!!
  interrupts();
  int m1=0;
  int m2=0;

  ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
    M1count=0;
    M2count=0;
  }

  while((abs(m1)<distance || abs(m2)<distance)&&(Sensor1.readRangeContinuousMillimeters()+offset_TOF1>60)){
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
        m1=M1count;
        m2=M2count;
    }

    if(abs(m1)>distance)md.setM1Brake(400);
    else md.setM1Speed(speed1);
    if(abs(m2)>distance)md.setM2Brake(400);
    else md.setM2Speed(speed2);
    
  }
  md.setBrakes(400, 400);  //if above brakes fails
  //Serial2.println("|##-> moved fixed distance! ");

}

double calculate_tof_error_left(double _tof_front_r,double _tof_back_r){
//,double _tof_front_l,double _tof_back_l
  double error=0;

  double left_error = _tof_front_r - _tof_back_r;
  //double left_error = _tof_front_l - _tof_back_l;
  if (_tof_front_r < offset_distance && _tof_back_r<offset_distance){
    error = -(offset_distance - left_error);// + left_error;
  }
  else if (_tof_front_r>offset_distance && _tof_back_r>offset_distance){
    error = (offset_distance + left_error);// - left_error;
  }
  else{
    error = KW*(left_error);// - left_error);
  }
  return error;
}

double calculate_tof_error_right(double _tof_front_r,double _tof_back_r){
//,double _tof_front_l,double _tof_back_l
  double error=0;

  double right_error = _tof_front_r - _tof_back_r;
  //double left_error = _tof_front_l - _tof_back_l;
  if (_tof_front_r < offset_distance && _tof_back_r<offset_distance){
    error = (offset_distance - right_error);// + left_error;
  }
  else if (_tof_front_r>offset_distance && _tof_back_r>offset_distance){
    error = -(offset_distance + right_error);// - right_error;
  }
  else{
    error = -KW*(right_error);// - left_error);
  }
  return error;
}

void align_left(){
  int m1_speed = default_m1_speed;
  int m2_speed = default_m2_speed;

  float tof_2 = Sensor4.readRangeContinuousMillimeters()+offset_TOF4;
  float tof_1 = Sensor5.readRangeContinuousMillimeters()+offset_TOF5;

 double left_tof_error = calculate_tof_error_left(tof_1,tof_2);

  while (Sensor1.readRangeContinuousMillimeters()+offset_TOF1>80 && abs(left_tof_error)>2){
     tof_2 = Sensor4.readRangeContinuousMillimeters()+offset_TOF4;
     tof_1 = Sensor5.readRangeContinuousMillimeters()+offset_TOF5;

     left_tof_error = calculate_tof_error_left(tof_1,tof_2);

      m1_speed += left_tof_error;
      m2_speed -= left_tof_error;

      if (m1_speed>400){
          m1_speed = 400;
      }else if (m1_speed<150){
          m1_speed = 150;
      }

      if (m2_speed>400){
          m2_speed = 400;
      }else if (m2_speed<150){
          m2_speed = 150;
      }
      md.setM1Speed(m1_speed);
      md.setM2Speed(m2_speed);   

  }
   md.setBrakes(400,400);
}

void align_right(){
  int m1_speed = default_m1_speed;
  int m2_speed = default_m2_speed;

  float tof_2 = Sensor3.readRangeContinuousMillimeters()+offset_TOF3;
  float tof_1 = Sensor2.readRangeContinuousMillimeters()+offset_TOF2;

  double right_tof_error = calculate_tof_error_right(tof_1,tof_2);

  while (Sensor1.readRangeContinuousMillimeters()+offset_TOF1>80 && abs(right_tof_error)>2){
     tof_2 = Sensor3.readRangeContinuousMillimeters()+offset_TOF3;
     tof_1 = Sensor2.readRangeContinuousMillimeters()+offset_TOF2;

     right_tof_error = calculate_tof_error_right(tof_1,tof_2);

      m1_speed += right_tof_error;
      m2_speed -= right_tof_error;

      if (m1_speed>400){
          m1_speed = 400;
      }else if (m1_speed<150){
          m1_speed = 150;
      }

      if (m2_speed>400){
          m2_speed = 400;
      }else if (m2_speed<150){
          m2_speed = 150;
      }
      md.setM1Speed(m1_speed);
      md.setM2Speed(m2_speed);   

  }
   md.setBrakes(400,400);
}

bool wall_in_left(){
    float tof_2 = Sensor4.readRangeContinuousMillimeters()+offset_TOF4;
    float tof_1 = Sensor5.readRangeContinuousMillimeters()+offset_TOF5;

    if(tof_1<200 && tof_2<200){
      _LED_all_off();
      LED3.on();
      return true;
    
    }
    return false;
  

}

bool wall_in_right(){
    float tof_2 = Sensor2.readRangeContinuousMillimeters()+offset_TOF2;
    float tof_1 = Sensor3.readRangeContinuousMillimeters()+offset_TOF3;

    if(tof_1<200 && tof_2<200){
      _LED_all_off();
      LED5.on();
      return true;
    }else{
      return false;
    }
}

void move_fixed_distance_with_walls(int distance){
    LED1.on();
    interrupts();
    int m1=0;
    int m2=0;

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
      M1count=0;
      M2count=0;
    }

    while((abs(m1)<distance || abs(m2)<distance)&&(Sensor1.readRangeContinuousMillimeters()+offset_TOF1>80)){
        if(wall_in_left()){
          Serial2.println("! left wall deteted");
          align_left();
        }else if(wall_in_right()){
          Serial2.println("! right wall deteted");
          align_right();
          
        }

        move_fixed_distance_with_tof(50);

        ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
          m1=M1count;
          m2=M2count;
        }
    }
    LED1.off();

}
///avoid using
void move_fixed_distance2(int distance, int slaveSpeed=default_m1_speed,int masterSpeed=default_m2_speed ){
  interrupts();
  int totalTicks = 0;
  int error = 0;
  int kp = 5;
 
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
    M2count = 0;
    M1count = 0;
  }
  while(abs(totalTicks) < distance)
  {
    //Proportional algorithm to keep the robot going straight.
    md.setM1Speed(masterSpeed);
    md.setM2Speed(slaveSpeed);
 
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
      error = M1count - M2count;
      slaveSpeed += error / kp;
 
      M2count = 0;
      M1count = 0;
    }
 
    
    delay(100);
 
    //Add this iteration's encoder values to totalTicks.
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
      totalTicks+= M1count;
    }
  }
  md.setBrakes(400,400);


}

void move_fixed_distance_pid(int distance ,int speed1=default_m1_speed,int speed2=default_m2_speed){
  interrupts();
  M1count=0;
  M2count=0;

  int last_error=0;


  while(abs(M1count)<distance || abs(M2count)<distance){

    int error=(abs(M1count)-abs(M2count))*10;    
    error=error+(error-last_error)*0;
    last_error = error;

    if(abs(M1count)>distance)md.setM1Brake(400);
    else {
      speed1=speed1-error;
      if(speed1<0){
        speed1=0;
      }
      if(speed1>400){
        speed1=400;
      }
      md.setM1Speed(speed1);

    }
    if(abs(M2count)>distance)md.setM2Brake(400);
    else{
      speed2=speed2+error;
      md.setM2Speed(speed2);
      if(speed2<0){
        speed2=0;
      }
      if(speed2>400){
        speed2=400;
      }
    }

    
  }
  md.setBrakes(400, 400);  //if above brakes fails

}

void make_90_degree_anticlockwise(int speed1=default_m1_speed,int speed2=default_m2_speed){
  

  /* --> To turn using Delays
   md.setM1Speed(speed1);
   md.setM2Speed(-speed2);
   delay(433);
   md.setBrakes(400,400);*/

  //turn using encoder counts
  move_fixed_distance(1050,speed1,-speed2);
  Serial2.println("turned anticlockwise 90 degrees");

}


void make_90_degree_clockwise(int speed1=default_m1_speed,int speed2=default_m2_speed){

  /*   --> turn using delays
   md.setM1Speed(-speed1);
   md.setM2Speed(speed2);
   delay(433);
   md.setBrakes(400,400); */

  //turn using encoders
  move_fixed_distance(1050,-speed1,speed2);
  Serial2.println("turned clockwise 90 degrees");

}

void make_45_degree_anticlockwise(int speed1=default_m1_speed,int speed2=default_m2_speed){
  

  /* --> To turn using Delays
   md.setM1Speed(speed1);
   md.setM2Speed(-speed2);
   delay(433);
   md.setBrakes(400,400);*/

  //turn using encoder counts
  move_fixed_distance(550,speed1,-speed2);  //was 525
  Serial2.println("turned anticlockwise 45 degrees");

}

void make_45_degree_clockwise(int speed1=default_m1_speed,int speed2=default_m2_speed){
  

  /* --> To turn using Delays
   md.setM1Speed(speed1);
   md.setM2Speed(-speed2);
   delay(433);
   md.setBrakes(400,400);*/

  //turn using encoder counts
  move_fixed_distance(550,-speed1,speed2);  //was 525
  Serial2.println("turned clockwise 45 degrees");

}

void move_cell_distance(int speed1=default_m1_speed,int speed2=default_m2_speed){
  //for(int i=0;i<3;i++){
    move_fixed_distance(10);
 /* for(int i=0;i<1010;i++){
    move_fixed_distance(1,speed1,-speed2);
  }*/
 
}

void make_x_degree_clockwise(int dis=0,int speed1=default_m1_speed,int speed2=default_m1_speed){
  move_fixed_distance(dis,-speed1,speed2);
}
void make_x_degree_anticlockwise(int dis=0,int speed1=default_m1_speed,int speed2=default_m2_speed){
  move_fixed_distance(dis,speed1,-speed2);
}

void coin_pick(){
   coin_servo_pos(1500);
   delay(2000);
}
void coin_place(){
    coin_servo_pos(1800);
    delay(2000);
}
