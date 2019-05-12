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
  Serial2.print(Sensor3.readRangeContinuousMillimeters());
  Serial2.print(",  ");
  Serial2.print(Sensor4.readRangeContinuousMillimeters());
  Serial2.print(",  \n");
  //Serial.print(Sensor4.readRangeContinuousMillimeters());
  //Serial.print(",  ");
  //Serial.print(Sensor5.readRangeContinuousMillimeters());
  //Serial.print(" \n  ");

  
  delay(x);
  
}
