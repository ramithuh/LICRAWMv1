void boot(){

    //essential at start to make use of Software Reset
    pinMode(RESET_PIN, INPUT);    // Just to be clear, as default is INPUT. Not really needed.
    digitalWrite(RESET_PIN, LOW); // Prime it, but does not actually set output. 


    pinMode(openmv_p7, INPUT_PULLUP);
    pinMode(openmv_p8, INPUT_PULLUP);
    pinMode(openmv_p9, INPUT_PULLUP);
 

    _LED_all_off();         //turn off all LEDs

    for(int i=0;i<10;i++){ //LED pattern to show that it's booting
        digitalWrite(LED_3, HIGH);
        digitalWrite(LED_5, HIGH);
        delay(100);
        digitalWrite(LED_3, LOW);
        digitalWrite(LED_5, LOW);
        delay(100);
    }

    Serial2.println(F("============================================================="));
    Serial2.println(F("|       X      Started booting  -  LICRAWM 1.5.4   X        |"));
    Serial2.println(F("============================================================="));                              
                                 

    _LED_all_off();
    
}

void _reset_board(){
    pinMode(RESET_PIN, OUTPUT); 
}

void boot_tof(){  //under construction
  pinMode(XSHUT_pin1, OUTPUT);
  pinMode(XSHUT_pin2, OUTPUT);
  pinMode(XSHUT_pin3, OUTPUT);
  pinMode(XSHUT_pin4, OUTPUT);
  pinMode(XSHUT_pin5, OUTPUT);
  


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

  pinMode(XSHUT_pin1,INPUT);
  delay(10);
  Sensor1.setAddress(Sensor1_newAddress);
  
 // LED2.on(); //addresses set!!

  Sensor1.init();
  Serial2.print(F("> Sensor 1 ok\n"));
  Sensor2.init();
  Serial2.print(F("> Sensor 2 ok\n"));
  Sensor3.init();
  Serial2.print(F("> Sensor 3 ok\n"));
  Sensor4.init();
  Serial2.print(F("> Sensor 4 ok\n"));
  Sensor5.init();
  Serial2.print(F("> Sensor 5 ok\n"));
/*   Sensor4.init();
  Serial.print("Sensor 4 ok");
  Sensor5.init();
  Serial.print("Sensor 5 ok");
    */
    Sensor1.setTimeout(500);
    Sensor2.setTimeout(500);
    Sensor3.setTimeout(500);
    Sensor4.setTimeout(500);
    Sensor5.setTimeout(500);

    Sensor1.setMeasurementTimingBudget(35000);
    Sensor2.setMeasurementTimingBudget(35000);
    Sensor3.setMeasurementTimingBudget(35000);
    Sensor4.setMeasurementTimingBudget(35000);
    Sensor5.setMeasurementTimingBudget(35000);
   

    Sensor4.setSignalRateLimit(4);
  // increase laser pulse periods (defaults are 14 and 10 PCLKs)
    Sensor4.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 12);
    Sensor4.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 8);

    Sensor5.setSignalRateLimit(3);
  // increase laser pulse periods (defaults are 14 and 10 PCLKs)
    Sensor5.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 12);
    Sensor5.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 8);

    Sensor2.setSignalRateLimit(3);
  // increase laser pulse periods (defaults are 14 and 10 PCLKs)
    Sensor2.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 12);
    Sensor2.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange,8);

    Sensor1.setSignalRateLimit(3);
  // increase laser pulse periods (defaults are 14 and 10 PCLKs)
    Sensor1.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 12);
    Sensor1.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange,8);
    
    Sensor3.setSignalRateLimit(3);
  // increase laser pulse periods (defaults are 14 and 10 PCLKs)
    Sensor3.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 12);
    Sensor3.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 8);

    Sensor1.startContinuous();
    Sensor2.startContinuous();
    Sensor3.startContinuous();
    Sensor4.startContinuous();
    Sensor5.startContinuous();

    Serial2.println(F("============================================================="));
    Serial2.println(F("|      \\      Successfully Booted ToFs     /                |"));
    Serial2.println(F("============================================================="));    
}

void boot_gyro(){

  setUpMPU();
  callibrateGyroValues();

  /*
  setUpMPU();
  callibrateGyroValues();*/
   /*  mpu6050.begin();
    delay(500);
    mpu6050.calcGyroOffsets(true)

  if(mpu6050.getAngleY()==90.00 && mpu6050.getAngleX()==-90.00){
    Serial2.println(F("============================================================="));
    Serial2.println(F("|      !!      !! GYRO NOT DETECTED !!      !!             |"));
    Serial2.println(F("=============================================================")); 
  }else{
    Serial2.println(F("============================================================="));
    Serial2.println(F("|      \\      Successfully Booted GYRO      /               |"));
    Serial2.println(F("=============================================================")); 
    LED3.on(); //booting gyro done!  
  } */
}

void boot_encoders(){
  pinMode(M1_ENCODER_A, INPUT);
  pinMode(M1_ENCODER_B, INPUT);
  pinMode(M2_ENCODER_A, INPUT); 
  pinMode(M2_ENCODER_B, INPUT);
  
  attachInterrupt(digitalPinToInterrupt(2),  rightEncoderEvent, CHANGE);
  attachInterrupt(digitalPinToInterrupt(18), leftEncoderEvent, CHANGE);
}

void boot_motors(){
    md.init();
}
void block_pid_print(int _mode){
  m1_global_speed=275;
  m2_global_speed=275;
  last_error = 0;
  _last_position=0;

  if(_mode==1){
      Serial2.println(F("====================================================="));
      Serial2.print(F("||||||||||||    New KP Value  =  "));
      Serial2.print(KP);
      Serial2.println(F("   ||||||||||||"));
      Serial2.println(F("====================================================="));
  }else if(_mode==0){
      Serial2.println(F("====================================================="));
      Serial2.print(F("||||||||||||    New KI Value  =  "));
      
      Serial2.print(KI);
      Serial2.println(F("   ||||||||||||"));
      Serial2.println(F("====================================================="));
  }else if(_mode==-1){
      Serial2.println(F("====================================================="));
      Serial2.print(F("||||||||||||    New KD Value  =  "));
      
      Serial2.print(KD);
      Serial2.println(F("   ||||||||||||"));
      Serial2.println(F("====================================================="));

  }

}

void _input_check(){


  if(DEBUG_BLUETOOTH==true){
 
      if (Serial2.available()) {
        char x=Serial2.read();

        LED1.blink(50);
        LED1.blink(50);
        LED1.blink(50);
        LED1.blink(50);
        
        if (x=='R') {
          Serial2.println(F("====================================================="));
          Serial2.println(F("|     **     Restarting  LICRAWM 1.5.4   **         |"));
          Serial2.println(F("====================================================="));                                                        
          delay(1000);
          _reset_board();

        }else if (x=='g') {         //Toggle Show Gyro Reading 
            DEBUG_GYRO=!DEBUG_GYRO;
            LED3.toggle();
        }else if (x=='t') {        //Toggle Show ToF Readings 
            DEBUG_TOF=!DEBUG_TOF;
            LED5.toggle();
        }else if (x=='s') {        //Toggle Show Loop speed  
            DEBUG_SPEED=!DEBUG_SPEED;
            LED1.toggle();
           
        }else if (x=='b') {  //Toggle Blue LED
            LED5.toggle();
        }else if (x=='r') {  //Toggle Red LED
            LED1.toggle();
        }else if (x=='o') {  //Debugging off AND VISUALIZE!
            DEBUG_TOF=0;
            DEBUG_GYRO=0;
            DEBUG_SPEED=0;
            DEBUG_ENCODERS=0;
            VISUALIZE=1;
            _LED_all_off();
            LED4.on();
        }else if(x=='v'){
          VISUALIZE=!VISUALIZE;

        }else if (x=='/') {         //Disable Bluetooth Reading
          DEBUG_BLUETOOTH=0;
        }else if(x=='T'){
          UPDATE_TOF=!UPDATE_TOF;   //Disable ToF 
        }else if(x=='G'){
          UPDATE_GYRO=!UPDATE_GYRO; //Disable Gyro
        }else if(x=='p'){
            Serial2.println(F("====================================================="));
            Serial2.println(F("============     KP       Edit Mode    =============="));
            Serial2.println(F("====================================================="));
            kp_mode=true;
            ki_mode=false;
            kd_mode=false;
        }else if(x=='i'){
            Serial2.println(F("====================================================="));
            Serial2.println(F("============     KI       Edit Mode    =============="));
            Serial2.println(F("====================================================="));
            kp_mode=false;
            ki_mode=true;
            kd_mode=false;
        }else if(x=='d'){
            Serial2.println(F("====================================================="));
            Serial2.println(F("============     KD       Edit Mode    =============="));
            Serial2.println(F("====================================================="));
            kp_mode=false;
            ki_mode=false;
            kd_mode=true;
        }else if(x=='+'){
            if(kp_mode){KP+=1; block_pid_print(1);}
            if(ki_mode){KI+=1; block_pid_print(0);}
            if(kd_mode){KD+=1; block_pid_print(-1);}

        }else if(x=='-'){
            if(kp_mode){KP-=1;block_pid_print(1);}
            if(ki_mode){KI-=1;block_pid_print(0);}
            if(kd_mode){KD-=1; block_pid_print(-1);}
        }else if(x=='['){
            //make_90_degree_anticlockwise();

            move_fixed_distance(1050,default_m1_speed,-default_m2_speed);
            Serial2.println("| Made -90 turn!");
           

        }else if(x==']'){
            //make_90_degree_clockwise();
            move_fixed_distance(1050,-default_m1_speed,default_m2_speed);
            Serial2.println("| Made +90 turn!");
           
            
        }else if(x=='^'){
           
            move_fixed_distance(1000);
            Serial2.println("| Move Fixed Distance");
           
            
        }else if(x=='n'){
           move_fixed_distance_pid(4010,-default_m1_speed,default_m2_speed);  
        
        }else if(x=='*'){
            if(FOLLOW_LINE==1){
                  md.setM1Speed(0);
                  md.setM2Speed(0);
                  m1_global_speed=275;
                  m2_global_speed=275;
                  last_error = 0;
                  _last_position=0;
                  
            }
            FOLLOW_LINE=!FOLLOW_LINE;
            
        }else if(x=='_'){
            coin_pick();
        }else if(x=='z'){
            coin_place();
        }
        
    }
  }
}



void openmv_serial_check(){
  if (Serial3.available() > 0) { //something sent by OpenMV
    String x=Serial3.readString();    
      Serial2.print("== + ");   //write it to bluetooth serial
      Serial2.print(x);
      Serial2.println(" +  ==");                  

  }
}                                            
                                                        
int openmv_digital_decode(){
  int p7=digitalRead(openmv_p7);
  int p8=digitalRead(openmv_p8);
  int p9=digitalRead(openmv_p9);

  
  if(p7==0 && p8==0 && p9==1){
    _LED_all_off();
   // LED2.blink();
    //LED2.blink(200);
    //LED2.blink(200);
    LED2.on();
    out="red";
    return 0;
  }
  if(p7==0 && p8==1 && p9==0){
    _LED_all_off();
   // LED3.blink();
    //LED3.blink(200);
    //LED3.blink(200);
    LED3.on();
    out="green";
    return 1;
  
  }
  if(p7==1 && p8==0 && p9==0){
    _LED_all_off();
   // LED5.blink();
   //LED5.blink(200);
    //LED5.blink(200);
    LED5.on();
    out= "blue";
    return 2;

  }
/*  if(out!=""){
      
      Serial2.print("== + ");   //write it to bluetooth serial
      Serial2.print(out);
      Serial2.print(" +  == :");    
      Serial2.println(String(p7)+String(p8)+String(p9));

  }else{*/
  _LED_all_off();
  return 0;
   


}


int read_colour(){
  long arr[3];
  arr[0]=0;  //RED
  arr[1]=0;  //GREEN
  arr[2]=0;  //BLUE
  unsigned long start = micros();

  while (micros()-start < 10000000){  //wait 15 seconds to check color
    arr[openmv_digital_decode()]+=1;
  }
  if (arr[0]>=arr[1]){
    if (arr[0]>=arr[2]){
      return 0;
    }else{
      return 2;
    }
  }else if (arr[1]>=arr[2]){
    return 1;
  }else{
    return 2;
  }
  
}

void boot_linearray(){
  LED2.blink(50);
  LED2.blink(50);
  LED2.on();
  Serial2.println(F("============================================================="));
  Serial2.println(F("|      \\  Started CALIBRATING     LINE ARRAY      /         |"));
  Serial2.println(F("=============================================================")); 

  for (uint16_t i = 0; i < 100; i++){
    linearray.calibrate();
  }

  Serial2.println(F("============================================================="));
  Serial2.println(F("|      \\  Successfully CALIBRATED LINE ARRAY      /         |"));
  Serial2.println(F("=============================================================")); 
  Serial2.print("|** ");
  for (uint8_t i = 0; i < SensorCount; i++){
    Serial2.print(linearray.calibrationOn.minimum[i]);
    Serial2.print(' ');
  }
  Serial2.println();

  // print the calibration maximum values measured when emitters were on
  Serial2.print("|** ");
  for (uint8_t i = 0; i < SensorCount; i++){
    Serial2.print(linearray.calibrationOn.maximum[i]);
    Serial2.print(' ');
  }
   Serial2.println();

  LED2.blink(500);
  LED2.blink(500);
  LED2.off();



}                                              
                                                        
                                                        
                                                        
                                                        