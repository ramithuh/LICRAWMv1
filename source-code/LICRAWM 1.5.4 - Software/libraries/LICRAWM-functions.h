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
   uint16_t r1,r2,r3,r4,r5;


  if(UPDATE_TOF){
    r2=Sensor2.readRangeContinuousMillimeters();
    r3=Sensor3.readRangeContinuousMillimeters();  //this is faster than single read
    r4=Sensor4.readRangeContinuousMillimeters();
    r5=Sensor5.readRangeContinuousMillimeters();

  }


  if(DEBUG_TOF==true){
  //Serial.print(Sensor1.readRangeContinuousMillimeters());
  //Serial.print(",  ");

        Serial2.print(F("T2:"));
        Serial2.print(r2);
        if (r2==65535){ Serial2.print(F(" Error: ToF2 -> TIMEOUT")); }

        Serial2.print(F(":T3:"));
        Serial2.print(r3);
        if (r3==65535){ Serial2.print(F(" Error: ToF3 -> TIMEOUT")); }

        Serial2.print(F(":T4:"));
        Serial2.print(r4);
        if (r4==65535) { Serial2.print(F(" Error: ToF4 -> TIMEOUT")); }

        Serial2.print(F(":T5:"));
        Serial2.print(r5);
        if (r5==65535){ Serial2.print(F(" Error: ToF5 -> TIMEOUT")); }

        Serial2.print(F("\n"));

        //Serial.print(Sensor4.readRangeContinuousMillimeters());
        //Serial.print(",  ");
        //Serial.print(Sensor5.readRangeContinuousMillimeters());
        //Serial.print(" \n  ");


        delay(miliseconds);
  }else if(!DEBUG_GYRO && !DEBUG_TOF ){
        out+="T2:";
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
        Serial2.print(F("angleX:"));
        Serial2.print(x);
        Serial2.print(F(":Y:"));
        Serial2.print(y);
        Serial2.print(F(":Z:"));
        Serial2.println(z);

        delay(miliseconds);
    }else if(!DEBUG_GYRO && !DEBUG_TOF){
        out+=":Z:";
        out+=z;
        out+=":Y:";
        out+=y;
        out+=":X:";
        out+=x;
    
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
