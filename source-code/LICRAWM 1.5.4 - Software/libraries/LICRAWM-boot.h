
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
   

    Sensor4.setSignalRateLimit(3);
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
    
    Sensor3.setSignalRateLimit(4);
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


    mpu6050.begin();
    mpu6050.calcGyroOffsets(true);

  if(mpu6050.getAngleY()==90.00 && mpu6050.getAngleX()==-90.00){
    Serial2.println(F("============================================================="));
    Serial2.println(F("|      !!      !! GYRO NOT DETECTED !!      !!             |"));
    Serial2.println(F("=============================================================")); 
  }else{
    Serial2.println(F("============================================================="));
    Serial2.println(F("|      \\      Successfully Booted GYRO      /               |"));
    Serial2.println(F("=============================================================")); 
    LED3.on(); //booting gyro done!  
  } 
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
void block_speed_print(bool m1){
  if(m1){
      Serial2.println(F("====================================================="));
      Serial2.print(F("||||||||||||    New M1 Speed  =  "));
      Serial2.print(m1_global_speed);
      Serial2.println(F("   ||||||||||||"));
      Serial2.println(F("====================================================="));
  }else{
      Serial2.println(F("====================================================="));
      Serial2.print(F("||||||||||||    New M2 Speed  =  "));
      
      Serial2.print(m2_global_speed);
      Serial2.println(F("   ||||||||||||"));
      Serial2.println(F("====================================================="));
  }

}

void _input_check(){

  /*
  if(S4.on()){
    DEBUG_TOF=0;
    DEBUG_GYRO=0;
    DEBUG_SPEED=0;
    _LED_all_off();
    LED4.on();
    
  }else{
    DEBUG_GYRO=1;
    LED3.on();
    DEBUG_TOF=1;
    LED4.off();
    LED5.on();

  }*/

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
        }else if(x=='1'){
            Serial2.println(F("====================================================="));
            Serial2.println(F("============     M1 Speed Edit Mode    =============="));
            Serial2.println(F("====================================================="));
            m1_mode=true;
            m2_mode=false;
        }else if(x=='2'){
            Serial2.println(F("====================================================="));
            Serial2.println(F("============     M2 Speed Edit Mode    =============="));
            Serial2.println(F("====================================================="));
            m1_mode=false;
            m2_mode=true;
        }else if(x=='+'){
            if(m1_mode){m1_global_speed+=10; block_speed_print(1);}
            if(m2_mode){m2_global_speed+=10; block_speed_print(0);}

        }else if(x=='-'){
            if(m1_mode){m1_global_speed-=10;block_speed_print(1);}
            if(m2_mode){m2_global_speed-=10;block_speed_print(0);}
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
                                                        
void openmv_digital_decode(){
  int p7=digitalRead(openmv_p7);
  int p8=digitalRead(openmv_p8);
  int p9=digitalRead(openmv_p9);

  String out="";
  if(p7==0 && p8==0 && p9==1)out="red";
  if(p7==0 && p8==1 && p9==0)out="green";
  if(p7==1 && p8==0 && p9==0)out= "blue";

  if(out!=""){
      Serial2.print("== + ");   //write it to bluetooth serial
      Serial2.print(out);
      Serial2.print(" +  == :");    
      Serial2.println(String(p7)+String(p8)+String(p9));

  }

}                                                     
                                                        
                                                        
                                                        
                                                        
                                                        