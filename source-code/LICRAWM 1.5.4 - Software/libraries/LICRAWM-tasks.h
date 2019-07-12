float calculate_pos(int threshold = 300){      
    /*

    Args:
        threshold: line_follow threshold
       
    Returns:
        return 0 -> if flag detected
        error: (error for pid)
     */
    
  /* if (threshold > 300){LED2.on();}
  else {LED2.off();}
*/
  linearray.readLineWhite(sensorValues);
  /*out="T1:0:T2:0:T3:0:T4:0:T5:0:Z:0:Y:0:X:0:M1:0:M2:0";

  out+=":L:";
  for (uint8_t i = 0; i < SensorCount; i++){
    out+=sensorValues[i];
    out+=":";
  }*/

  int pos=0;
  int on_count=0;
  for(int i=0;i<15;i++){
      if(sensorValues[i]<threshold){
          pos+=(i-7)*10;
          on_count+=1;
      }
  }

  /* out+=":POS:";
  out+=pos;
  out+=":T:";
  out+=threshold;

  Serial2.println(out);
*/
  if(on_count>10){
    Serial2.print("On count: ");
    Serial2.println(on_count);
   
    md.setBrakes(300,300);
      
    if (digitalRead(LEFT_TRACKER)==1 && digitalRead(RIGHT_TRACKER)==1){      //Color is in FOV, or ALL sensors on!
      md.setBrakes(300,300);
      flag = 1;
      flag_count += 1;
      Serial2.print("| flag_count :");
      Serial2.print(flag_count);
      Serial2.print(" on_sensor_count: ");
      Serial2.print(on_count);
      Serial2.print(" threshold: ");
      Serial2.println(threshold);
    }
    else if (digitalRead(LEFT_TRACKER)==0 && digitalRead(RIGHT_TRACKER)==1){    //sensorValues[0]<threshold && sensorValues[14]>threshold
      move_fixed_distance(700);
      delay(200);
      make_90_degree_anticlockwise(180,210);
      md.setBrakes(300, 300);
      delay(200);
    }
    else if (digitalRead(LEFT_TRACKER)==1 && digitalRead(RIGHT_TRACKER)==0){
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

void line_follow(int distance=0){
    /*
    Follow line indefinitely
     */

  _LED_all_off();
  LED1.on();
  if (distance==0){
    while(1){
      md.setM1Speed(m1_global_speed);
      md.setM2Speed(m2_global_speed);
      
    
      int position;
      
      int last_flag_count = flag_count;
      if(flag_count==2 || flag_count==3){
          position=calculate_pos(700);
      }else{
          position=calculate_pos();
      }
    
      if (last_flag_count < flag_count){
        _LED_all_off();
        return;
      }

      //calculating Error
      int error = position ;

      //PID controlling
      float motor_speed = KP*error + KI*(error + last_error) + KD*(error - last_error);
      // Serial2.print(" :motor_speed_before_mapping:");
      // Serial2.print(motor_speed);

      last_error = error;

      m1_global_speed = m1_global_speed - motor_speed;    //plus or minus may change accordingly
      m2_global_speed = m2_global_speed + motor_speed;

      if (m1_global_speed>230){
          m1_global_speed = 230;
      }
      else if (m1_global_speed<47){
          m1_global_speed = 47;
      }
      if (m2_global_speed>243){
          m2_global_speed = 243;
      }
      else if (m2_global_speed<60){
          m2_global_speed = 60;
      }
    }
  }
  else{
    int m1=0;
    int m2=0;

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
      M1count=0;
      M2count=0;
    }
    
    while((m1<distance)&&(m2<distance)){
      md.setM1Speed(m1_global_speed);
      md.setM2Speed(m2_global_speed);
    
      int position;
      position=calculate_pos();
      //calculating Error
      int error = position ;

      //PID controlling
      float motor_speed = KP*error + KI*(error + last_error) + KD*(error - last_error);
      // Serial2.print(" :motor_speed_before_mapping:");
      // Serial2.print(motor_speed);

      last_error = error;

      m1_global_speed = m1_global_speed - motor_speed;    //plus or minus may change accordingly
      m2_global_speed = m2_global_speed + motor_speed;

      if (m1_global_speed>230){
          m1_global_speed = 230;
      }
      else if (m1_global_speed<60){
          m1_global_speed = 60;
      }
      if (m2_global_speed>230){
          m2_global_speed = 230;
      }
      else if (m2_global_speed<60){
          m2_global_speed = 60;
      }
      ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
        m1=M1count;
        m2=M2count;
      }    
    }
  }
}

void FOLLOW_WALL() {
    /* 
    Follow wall indefinitely

    */

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

void water_transfer(){
    /*
     Water transfer
    */
    
    make_90_degree_clockwise();  //turn 180 degrees to transfer water from the back
    delay(1000);
    make_90_degree_clockwise();         

    //move forward to align with the line
    line_follow(2000);

    //reverse to the containers
    move_fixed_distance(1600,-default_m1_speed,-default_m2_speed);

    //lowering the arm
    for (int j = 2000;j>=1475;j-=25){
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

void coin_collect() {
  //Coin collecting task
  md.setBrakes(300,300);
  Serial2.println("| Going to Pick Coin");
  Serial2.println("|   Reversed 4.5cm");
  move_fixed_distance(700,-default_m1_speed,-default_m2_speed);//4.5 cm reverse 

  Serial2.println("|   Reading Color for 15s....");
  coin_colour = read_colour();

  Serial2.print("|   Color is ");
  Serial2.println(coin_colour);

  Serial2.println("|   Moving forward to pick!");
}