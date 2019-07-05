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
    double x=9;
    double y=9;
    double z=9;
    if(UPDATE_GYRO){
       /*readAndProcessAccelData();
       readAndProcessGyroData();
       calculateAngle();*/
       
        mpu6050.update(); 


        /* 
        x=mpu6050.getAngleX();
        y=mpu6050.getAngleY();
        z=mpu6050.getAngleZ();*/
        x=mpu6050.getGyroAngleX();
        y=mpu6050.getGyroAngleY();
        z=mpu6050.getGyroAngleZ();
    }

    
    if(DEBUG_GYRO==true){      /* displays only if needed */
        Serial2.print(F("angleX:"));
        Serial2.print(x);
        Serial2.print(F(":Y:"));
        Serial2.print(y);
        Serial2.print(F(":Z:"));
        Serial2.println(z);
        delay(miliseconds);
    }
    if(VISUALIZE){
        out+=":Z:";
        out+=z;
        out+=":Y:";
        out+=y;
        out+=":X:";
        out+=x;
    
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

void arm_position(int pivot=0,int tilt=0,int grip=0,int miliseconds=0){
    pivot_servo.writeMicroseconds(pivot);
    delay(miliseconds);
    tilt_servo.writeMicroseconds(tilt);
    delay(miliseconds);
    grip_servo.writeMicroseconds(grip);
    delay(miliseconds);   
}

void transfer_water(int water_transfer_time=0){
    //place the straw
    arm_position(1450,1550,600,1000);
    //wait till water is transferred
    delay(water_transfer_time);
    //bring the arm to its initial position
    arm_position(1000,1200,2400,1000);
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

void move_fixed_distance(int distance ,int speed1=m1_global_speed,int speed2=m2_global_speed){
  M1count=0;
  M2count=0;
  long old_m1_count=abs(M1count);
  long old_m2_count=abs(M2count);

  while(abs(abs(M1count)-old_m1_count)<distance || abs(abs(M2count)-old_m2_count)<distance){

  /*  Serial2.print("|##-> moving fixed distance M1:");
    Serial2.print(M1count);
    Serial2.print(" M2:");
    Serial2.println(M1count);*/
  

    if(abs(abs(M1count)-old_m1_count)>distance)md.setM1Brake(400);
    else md.setM1Speed(speed1);
    if(abs(abs(M2count)-old_m2_count)>distance)md.setM2Brake(400);
    else md.setM2Speed(speed2);
    
  }
  md.setBrakes(400, 400);  //if above brakes fails
  Serial2.println("|##-> moved fixed distance! ");


}

void move_fixed_distance2(int distance, int slaveSpeed=m1_global_speed,int masterSpeed=m2_global_speed ){
  int totalTicks = 0;
  int error = 0;
  int kp = 5;
 
  M2count = 0;
  M1count = 0;
 
  while(abs(totalTicks) < distance)
  {
    //Proportional algorithm to keep the robot going straight.
    md.setM2Speed(masterSpeed);
    md.setM1Speed(slaveSpeed);
 
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
      error = M2count - M1count;
    }
    
 
    slaveSpeed += error / kp;
 
    M2count = 0;
    M1count = 0;
 
    delay(100);
 
    //Add this iteration's encoder values to totalTicks.
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
      totalTicks+= M2count;
    }
  }
  md.setBrakes(400,400);

}

void move_fixed_distance_pid(int distance ,int speed1=m1_global_speed,int speed2=m2_global_speed){
  M1count=0;
  M2count=0;

  int last_error=0;


  while(abs(M1count)<distance || abs(M2count)<distance){

    int error=(abs(M1count)-abs(M2count))*10;    
    error=error+(error-last_error)*0;
    last_error = error;


    Serial2.print("error:");
    Serial2.println(error);


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

void make_90_degree_clockwise(int speed1=m1_global_speed,int speed2=m2_global_speed){
  /* for(int i=0;i<101;i++){
    Serial2.println("clockwise");
    Serial2.println(i);
   move_fixed_distance(10,-speed1,speed2);
  }*/
   /* for(int i=0;i<1010;i++){
   move_fixed_distance(1,-speed1,speed2);
  }*/
 move_fixed_distance2(955,-400,400);
}
void make_90_degree_anticlockwise(int speed1=m1_global_speed,int speed2=m2_global_speed){
  /*for(int i=0;i<101;i++){
      Serial2.println("anti clockwise");
    Serial2.println(i);
    move_fixed_distance(10,speed1,-speed2);
  }*/
 /* for(int i=0;i<1010;i++){
    move_fixed_distance(1,speed1,-speed2);
  }*/
  move_fixed_distance2(955,400,-400);
}
void move_cell_distance(int speed1=m1_global_speed,int speed2=m2_global_speed){
  //for(int i=0;i<3;i++){
    move_fixed_distance(10);
 /* for(int i=0;i<1010;i++){
    move_fixed_distance(1,speed1,-speed2);
  }*/
 
}

void make_x_degree_clockwise(int dis=0,int speed1=m1_global_speed,int speed2=m2_global_speed){
  move_fixed_distance(dis,-speed1,speed2);
}
void make_x_degree_anticlockwise(int dis=0,int speed1=m1_global_speed,int speed2=m2_global_speed){
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
double calculate_tof_error(double _tof_front_r,double _tof_back_r){
//,double _tof_front_l,double _tof_back_l
  double error=0;
  double right_error = _tof_front_r - _tof_back_r;
  //double left_error = _tof_front_l - _tof_back_l;
  if (_tof_front_r<offset_distance && _tof_back_r<offset_distance){
    error = offset_distance - right_error;// + left_error;
  }
  else if (_tof_front_r>offset_distance && _tof_back_r>offset_distance){
    error = -1*(offset_distance + right_error);// - left_error;
  }
  else{
    error = -1*KW*(right_error);// - left_error);
  }
  return error;
}