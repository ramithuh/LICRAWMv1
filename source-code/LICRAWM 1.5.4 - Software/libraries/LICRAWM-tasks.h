int calculate_pos(int threshold = 300){      
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


 //out="T1:0:T2:0:T3:0:T4:0:T5:0:Z:0:Y:0:X:0:M1:0:M2:0";

 /*  out+=":L:";
  for (uint8_t i = 0; i < SensorCount; i++){
    out+=sensorValues[i];
    out+=":";
  } 
*/
  int pos=0;
  int on_count=0;
  for(int i=0;i<16;i++){
      if(sensorValues[i]<threshold){
          //pos+=(i-8)*10;
          pos+=error_weights[i];
          on_count+=1;
      }
  }
/* 
  out+=":on_count:";
  out+=on_count;
  out+=":T:";
  out+=threshold; 

  Serial2.println(out);
*/
  if(on_count>8){
    //Serial2.print("On count: ");
    //Serial2.println(on_count);
    bool LEFT_TRACKER_1=digitalRead(LEFT_TRACKER);
    bool RIGHT_TRACKER_1=digitalRead(RIGHT_TRACKER);
  
      
    if ((LEFT_TRACKER_1==1 && RIGHT_TRACKER_1==1) && MAZE_SOLVED==0 ){      //Color is in FOV, or ALL sensors on!
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
    else if(LEFT_TRACKER_1==0 && RIGHT_TRACKER_1==0 ){
      flag_count += 1;
      Serial2.print("After Wall Follow");
      Serial2.println(" on_sensor_count: ");
      Serial2.print(on_count);
    }
    else if (LEFT_TRACKER_1==0 && RIGHT_TRACKER_1==1 && WATER_TRANSFERRED==0 ){    //sensorValues[0]<threshold && sensorValues[14]>threshold
      count_90+=1;
      move_fixed_distance(900);
      
      make_90_degree_anticlockwise(180,210);
      md.setBrakes(300, 300);
      delay(200);
      
      
      
    }
    else if (LEFT_TRACKER_1==1 && RIGHT_TRACKER_1==0 ){
      count_90+=1;
      move_fixed_distance(900);
      
      make_90_degree_clockwise(180,210);
      md.setBrakes(300, 300);
      delay(200);
      
      
    }
    else if (threshold==501 && on_count>12){
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
  }


  if(on_count==0){
    bool LEFT_TRACKER_1=digitalRead(LEFT_TRACKER);
    bool RIGHT_TRACKER_1=digitalRead(RIGHT_TRACKER);
    /*if (MAZE_SOLVED==1){
      flag_count +=1;
    }
    else if (Sensor3.readRangeContinuousMillimeters()+offset_TOF3<80||Sensor4.readRangeContinuousMillimeters()+offset_TOF4<80 || Sensor1.readRangeContinuousMillimeters()+offset_TOF1<80){
      flag_count += 1;
    }*/
    if (RIGHT_TRACKER_1==1 && LEFT_TRACKER_1==1  && count_90>=4 ){
      flag_count+=1;
      //Serial2.println("Water transfer/Maze Enter!");
      return;
    }else{
      return _last_position;
    }
  }
  _last_position=pos/on_count;
  return _last_position;
  
  
}

void line_follow(int threshold=300,int distance=0){
    /*
    Follow line indefinitely
     */
  m1_global_speed=default_m1_speed;
  m2_global_speed=default_m2_speed;


  _LED_all_off();
  LED1.on();

  if (distance==0){
    while(1){
      md.setM1Speed(m1_global_speed);
      md.setM2Speed(m2_global_speed);
      
    
      int position;
      
      int last_flag_count = flag_count;
      position = calculate_pos(threshold);
    
      if (last_flag_count < flag_count){
        _LED_all_off();
        md.setBrakes(300,300);
        return;
      }
      

      //calculating Error
      int error = position ;

      //PID controlling
      int motor_speed = KP*error + KI*(error + last_error) + KD*(error - last_error);
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
      if (m2_global_speed>230){
          m2_global_speed = 230;
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
      int motor_speed = KP*error + KI*(error + last_error) + KD*(error - last_error);
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
    md.setBrakes(200,200);
  }
}

void FOLLOW_WALL() {
    /* 
    Follow wall indefinitely

    */
while(1){
    float tof1 = Sensor1.readRangeContinuousMillimeters()+offset_TOF1;
    float tof2 = Sensor2.readRangeContinuousMillimeters()+offset_TOF2;
    float tof3 = Sensor3.readRangeContinuousMillimeters()+offset_TOF3;
    float tof4 = Sensor4.readRangeContinuousMillimeters()+offset_TOF4;
    float tof5 = Sensor5.readRangeContinuousMillimeters()+offset_TOF5;

    linearray.readLineWhite(sensorValues);
    int on_count=0;
    for(int i=0;i<16;i++){
      if(sensorValues[i]<300){
        on_count+=1;
      }
    }
    if (on_count>0){
      return;
    }
    
   /*  out+="T1:";
    out+=tof1;
    out+=":T2:";
    out+='0';
    out+=":T3:";
    out+=tof3;
    out+=":T4:";
    out+=tof4;
    out+=":T5:";
    out+=tof5;*/
    
    if (tof4>160 && tof5>160){  // can I turn Left ??

            Serial2.println("#no wall in left");
           
            //move_fixed_distance_with_tof(1200);
            md.setBrakes(400,400);
            delay(2000);

            LED3.on();
            make_90_degree_anticlockwise();
            LED3.off();
            
            delay(1000);  
            move_fixed_distance_with_walls(2750);
            delay(1000);
            
    }else if (tof1>160){
            Serial2.println("#wall in left & NO wall in FRONT");
            _LED_all_off();
            md.setBrakes(400,400);
            move_fixed_distance_with_walls(2750);
            
            delay(1000);

    }else if (tof3>160){
            Serial2.println("#wall in left & front NOT in RIGHT");

            md.setBrakes(400,400);

            LED5.on();
            make_90_degree_clockwise();
            LED5.off();

            delay(1000);
            move_fixed_distance_with_walls(2750);   
            delay(1000);

    }else{
            md.setBrakes(300,300);
            LED2.on();
            make_90_degree_clockwise();
            delay(1000);
            make_90_degree_clockwise();
            delay(1000);
            LED2.on();

            move_fixed_distance_with_walls(2750);
            
            delay(1000);
    }
}


}

void water_transfer(){
    /*
     Water transfer
    */
    
    make_90_degree_anticlockwise();  //turn 180 degrees to transfer water from the back
    delay(2000);
    make_90_degree_anticlockwise();         
    delay(2000);
    //move forward to align with the line
    line_follow(300,50);
    delay(1000);

    //reverse to the containers
    move_fixed_distance(650,-default_m2_speed,-default_m1_speed);
    delay(2000);

    //lowering the arm
    lower_water_arm();

    delay(1000);      //delay to transfer water
    Serial2.println("Starting Water Transfer 7 HIGH");
    digitalWrite(7,HIGH);//start water transfer
    delay(10000);
    lift_water_arm(1675);
    delay(5000);
    Serial2.println("Ended Water Transfer 7 HIGH");
    digitalWrite(7,LOW);//finished transfering water
    shake_water_arm();
    delay(1000);
    arm_servo.writeMicroseconds(1750);
    //lifting the arm
    //lift_water_arm();


    delay(1000);    //wait till water is drained

} 

void coin_collect() {
  //Coin collecting task
  md.setBrakes(300,300);
  Serial2.println("| Going to Pick Coin");
  Serial2.println("|   Reversed 4.5cm");
  delay(1000);
  move_fixed_distance(600,-210,-180);
  //move_fixed_distance(700,-default_m2_speed,-default_m1_speed);//4.5 cm reverse 700

  Serial2.println("|   Reading Color for 15s....");
  coin_colour = read_colour();

  Serial2.print("|   Color is ");
  Serial2.println(coin_colour);

  Serial2.println("|   Moving forward to pick!");
  line_follow(300,50);
  delay(1000);
  line_follow(300);
  delay(1000);
  move_fixed_distance(1000,170,170);
  delay(3000);
  line_follow(300,50);
  delay(1000);
  line_follow(550);
  delay(5000);
}

void start_square(){
    // ####### START SQUARE! ######
 
  
  move_fixed_distance(1500);
  // #######        ######

}

void coin_drop(){
  md.setBrakes(300,300);
  Serial2.println("|  Coin Place |");
  move_fixed_distance(400);
  coin_place();
  move_fixed_distance(300);
}