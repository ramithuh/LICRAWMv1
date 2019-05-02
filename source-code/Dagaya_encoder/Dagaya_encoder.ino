

#include <DualVNH5019MotorShield.h>

DualVNH5019MotorShield md(38,39,5,40,A1,35,36,4,37,A0);

//DualVNH5019MotorShield md(35,36,4,37,A0,38,39,5,40,A1);
#define M1_ENCODER_A 2  //encoder pins MOTORB
#define M1_ENCODER_B 3

#define M2_ENCODER_A 18
#define M2_ENCODER_B 19


volatile unsigned long leftCount = 0;
volatile unsigned long rightCount = 0;



void setup() {


  pinMode(M1_ENCODER_A, INPUT);
  pinMode(M1_ENCODER_B, INPUT);
  pinMode(M2_ENCODER_A, INPUT); 
  pinMode(M2_ENCODER_B, INPUT);
  
  Serial.begin(9600);//  seial monitor initialized 

  attachInterrupt(digitalPinToInterrupt(2), rightEncoderEvent, CHANGE);
  attachInterrupt(digitalPinToInterrupt(18), leftEncoderEvent, CHANGE);

  md.init();
}

void loop() {

  md.setM1Speed(70);
  md.setM2Speed(78);
  Serial.print("M1 Count: "); //(motor B)
  Serial.print(rightCount);
  Serial.print(" M2 Count: ");
  Serial.println(leftCount);


}

void rightEncoderEvent() {
  if (digitalRead(M1_ENCODER_A) == HIGH) {
    if (digitalRead(M1_ENCODER_B) == LOW) {
      rightCount++;
    } else {
      rightCount--;
    }
  } else {
    if (digitalRead(M1_ENCODER_B) == LOW) {
      rightCount--;
    } else {
      rightCount++;
    }
  }
}

void leftEncoderEvent() {
  if (digitalRead(M2_ENCODER_A) == HIGH) {
    if (digitalRead(M2_ENCODER_B) == LOW) {
      leftCount++;
    } else {
      leftCount--;
    }
  } else {
    if (digitalRead(M2_ENCODER_B) == LOW) {
      leftCount--;
    } else {
      leftCount++;
    }
  }
}
