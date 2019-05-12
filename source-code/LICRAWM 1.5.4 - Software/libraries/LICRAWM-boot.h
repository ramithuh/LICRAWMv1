void boot(){

    //essential at start to make use of Software Reset
    pinMode(RESET_PIN, INPUT);    // Just to be clear, as default is INPUT. Not really needed.
    digitalWrite(RESET_PIN, LOW); // Prime it, but does not actually set output. 

    _LED_all_off();         //turn off all LEDs

    for(int i=0;i<10;i++){ //LED pattern to show that it's booting
        digitalWrite(LED_3, HIGH);
        digitalWrite(LED_5, HIGH);
        delay(100);
        digitalWrite(LED_3, LOW);
        digitalWrite(LED_5, LOW);
        delay(100);
    }

    _LED_all_off();
    
}

void _reset_board(){
    pinMode(RESET_PIN, OUTPUT); 
}

void boot_tof(){
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
  
  LED2.on(); //addresses set!!

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
}