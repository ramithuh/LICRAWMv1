/* Contains LICRAWM BOARD FUNCTIONS */

#define TraceFunc()   do { Serial2.print(F("In function: ")); Serial2.println(__func__); } while (0)

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
   //uint16_t r3=Sensor3.readRangeSingleMillimeters();
   //uint16_t r4=Sensor4.readRangeSingleMillimeters();
   uint16_t r3;
   uint16_t r4;

  if(UPDATE_TOF){
    r3=Sensor3.readRangeContinuousMillimeters();  //this is faster than single read
    r4=Sensor4.readRangeContinuousMillimeters();
  }
  if(DEBUG_TOF==true){
  //Serial.print(Sensor1.readRangeContinuousMillimeters());
  //Serial.print(",  ");
        Serial2.print(F("ToF3:"));
        Serial2.print(r3);
        if (r3==65535){ Serial2.print(F(" Error: ToF3 -> TIMEOUT")); }


        Serial2.print(F(":ToF4:"));
        Serial2.print(r4);
        if (r4==65535) { Serial2.print(F(" Error: ToF4 -> TIMEOUT")); }
        Serial2.print(F("\n"));

        //Serial.print(Sensor4.readRangeContinuousMillimeters());
        //Serial.print(",  ");
        //Serial.print(Sensor5.readRangeContinuousMillimeters());
        //Serial.print(" \n  ");
        delay(miliseconds);
  }
  
}

void get_gyro_reading(int miliseconds=0){
    float x;
    float y;
    float z;
    if(UPDATE_GYRO){
        mpu6050.update(); 
        x=mpu6050.getAngleX();
        y=mpu6050.getAngleY();
        z=mpu6050.getAngleZ();
    }

    if(DEBUG_GYRO==true){      /* displays only if needed */
        Serial2.print(F("angleX :"));
        Serial2.print(x);
        Serial2.print(F(":angleY :"));
        Serial2.print(y);
        Serial2.print(F(":angleZ :"));
        Serial2.println(z);

        delay(miliseconds);
    }
}
