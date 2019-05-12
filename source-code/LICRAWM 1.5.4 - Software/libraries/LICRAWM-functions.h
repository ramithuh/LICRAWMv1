/* Contains LICRAWM BOARD FUNCTIONS */


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

void print_tof_reading(int x=0){

  
  //Serial.print(Sensor1.readRangeContinuousMillimeters());
  //Serial.print(",  ");
  Serial2.print("ToF3:");
  uint16_t r=Sensor3.readRangeContinuousMillimeters();
  Serial2.print(r);
  if (r==65535){ Serial2.print(" Error: ToF3 -> TIMEOUT"); }


  Serial2.print(", ToF4:");
  r=Sensor4.readRangeContinuousMillimeters();
  Serial2.print(r);
  if (r==65535) { Serial2.print(" Error: ToF4 -> TIMEOUT"); }
  Serial2.print(",  \n");
  //Serial.print(Sensor4.readRangeContinuousMillimeters());
  //Serial.print(",  ");
  //Serial.print(Sensor5.readRangeContinuousMillimeters());
  //Serial.print(" \n  ");

  
  delay(x);
  
}

void print_gyro_reading(int x=0){

    Serial2.print("angleX : ");
    Serial2.print(mpu6050.getAngleX());
    Serial2.print(" angleY : ");
    Serial2.print(mpu6050.getAngleY());
    Serial2.print(" angleZ : ");
    Serial2.println(mpu6050.getAngleZ());

    delay(x);
}
