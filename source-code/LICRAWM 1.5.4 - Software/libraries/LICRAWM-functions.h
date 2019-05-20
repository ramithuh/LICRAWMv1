/* Contains LICRAWM BOARD FUNCTIONS */

#define TraceFunc()   do { Serial2.print(F("In function: ")); Serial2.println(__func__); } while (0)

int freeRam() {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}


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
   float r1=999;
   float r2=999;
   float r3=999;
   float r4=999;
   float r5=999;

  if(UPDATE_TOF==1){
    r2=Sensor2.readRangeContinuousMillimeters();
    r3=Sensor3.readRangeContinuousMillimeters();  //this is faster than single read
    r4=Sensor4.readRangeContinuousMillimeters();
    r5=Sensor5.readRangeContinuousMillimeters();
  }

/*** smoothen  & calib **/
r2+=offset_TOF2;
r3+=offset_TOF3;
r4+=offset_TOF4;
r5+=offset_TOF5;

S_TOF4+=r4;
S_TOF5+=r5;
S_TOF3+=r3;
S_TOF2+=r2;
r4=S_TOF4.process().mean;
r5=S_TOF5.process().mean;
r3=S_TOF3.process().mean;
r2=S_TOF2.process().mean;

//** **/
  if(DEBUG_TOF==true){

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

        delay(miliseconds);

  }else if(VISUALIZE){
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
    float x=9;
    float y=9;
    float z=9;
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
    }
    if(VISUALIZE){
        out+=":Z:";
        out+=z;
        out+=":Y:";
        out+=y;
        out+=":X:";
        out+=x;
    
    }
}
void get_encoder_reading(){
  if(DEBUG_ENCODERS){
        Serial2.print("M1 Count: "); //(motor B)
        Serial2.print(M1count);
        Serial2.print(" M2 Count: ");
        Serial2.println(M2count);
  }

  if(VISUALIZE){
      out+=":M1:";
      out+=M1count;
      out+=":M2:";
      out+=M2count;

  }
}

void rightEncoderEvent() {
  if (digitalRead(M1_ENCODER_A) == HIGH) {
    if (digitalRead(M1_ENCODER_B) == LOW) {
      M1count++;
    } else {
      M1count--;
    }
  } else {
    if (digitalRead(M1_ENCODER_B) == LOW) {
      M1count--;
    } else {
      M1count++;
    }
  }
}
void leftEncoderEvent() {
  if (digitalRead(M2_ENCODER_A) == HIGH) {
    if (digitalRead(M2_ENCODER_B) == LOW) {
      M2count++;
    } else {
      M2count--;
    }
  } else {
    if (digitalRead(M2_ENCODER_B) == LOW) {
      M2count--;
    } else {
      M2count++;
    }
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
