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

    Serial2.println("=============================================================");
    Serial2.println("       X      Started booting  -  LICRAWM 1.5.4   X          ");
    Serial2.println("=============================================================");                              
                                 

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
  

 /* pinMode(XSHUT_pin5,INPUT);
  delay(10);
  Sensor5.setAddress(Sensor5_newAddress);
 
  pinMode(XSHUT_pin4,INPUT);
  delay(10);
  Sensor4.setAddress(Sensor4_newAddress);
  */
  pinMode(XSHUT_pin4,INPUT);
  delay(10);
  Sensor4.setAddress(Sensor4_newAddress);
  
  pinMode(XSHUT_pin3,INPUT);
  delay(10);
  Sensor3.setAddress(Sensor3_newAddress);
  
 // LED2.on(); //addresses set!!

  /*pinMode(XSHUT_pin1, INPUT);
  delay(10);
  Sensor1.setAddress(Sensor1_newAddress);*/
 
  //Sensor1.init();
  //Serial.print("Sensor 1 ok");
  Sensor3.init();
  Serial.print("Sensor 3 ok");
  Sensor4.init();
  Serial.print("Sensor 4 ok\n");
/*   Sensor4.init();
  Serial.print("Sensor 4 ok");
  Sensor5.init();
  Serial.print("Sensor 5 ok");
    */

    Sensor3.setTimeout(500);
    Sensor4.setTimeout(500);

    Sensor3.startContinuous();
    Sensor4.startContinuous();

    Serial2.println("=============================================================");
    Serial2.println("       \\      Successfully Booted ToFs     /                ");
    Serial2.println("=============================================================");    
}

void boot_gyro(){


    mpu6050.begin();
    mpu6050.calcGyroOffsets(true);

    Serial2.println("=============================================================");
    Serial2.println("       \\      Successfully Booted GYRO      /                ");
    Serial2.println("=============================================================");    
}

void bluetooth_input_check(){
  if(DEBUG_BLUETOOTH==true){
 
      if (Serial2.available()) {
        char x=Serial2.read();
        
        if (x=='R') {
          Serial2.println("=====================================================");
          Serial2.println("      **     Restarting  LICRAWM 1.5.4   **          ");
          Serial2.println("=====================================================");                                                        
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
        }else if (x=='o') {  //Debugging Disabled
            DEBUG_TOF=0;
            DEBUG_GYRO=0;
            DEBUG_SPEED=0;
            _LED_all_off();

        }else if (x=='/') {         //Disable Bluetooth Reading
          DEBUG_BLUETOOTH=0;
        }else if(x=='T'){
          UPDATE_TOF=!UPDATE_TOF;   //Disable ToF 
        }else if(x=='G'){
          UPDATE_GYRO=!UPDATE_GYRO; //Disable Gyro
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
                                                        
                                                        
                                                        
                                                        
                                                        