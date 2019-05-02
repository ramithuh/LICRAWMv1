

const int MotorPinA = 4; // direction motor 1 (Channel A)
const int MotorSpeedPinA = 5; // for motor 1 (channel A)
const int MotorPinB = 7; // direction motor 2 (Channel B)
const int MotorSpeedPinB = 6;// for motor 2 (channel B)


#define RH_ENCODER_A 2  //encoder pins MOTORB
#define RH_ENCODER_B 3

#define LH_ENCODER_A 18
#define LH_ENCODER_B 19


volatile unsigned long leftCount = 0;
volatile unsigned long rightCount = 0;

const int CW  = HIGH;
const int CCW = LOW;

void setup() {
  // motor A pin assignment
  pinMode(MotorPinA, OUTPUT);
  pinMode(MotorSpeedPinA, OUTPUT);


  // motor B pin assignment
  pinMode(MotorPinB, OUTPUT);
  pinMode(MotorSpeedPinB, OUTPUT);


  pinMode(RH_ENCODER_A, INPUT);
  pinMode(RH_ENCODER_B, INPUT);
  pinMode(LH_ENCODER_A, INPUT);
  pinMode(LH_ENCODER_B, INPUT);
  
  Serial.begin(9600);//  seial monitor initialized 

  attachInterrupt(digitalPinToInterrupt(2), rightEncoderEvent, CHANGE);
  attachInterrupt(digitalPinToInterrupt(18), leftEncoderEvent, CHANGE);
}

void loop() {


  moveMotor('B', CW, 250); 
  moveMotor('A', CW, 250); 
  Serial.print("Right Count: "); //(motor B)
  Serial.print(rightCount);
  Serial.print(" Left Count: ");
  Serial.println(leftCount);


}// loop end


/*
 * 
 * Written by Ahmad Shamshiri August 29 2018 at 20:59 in Ajax, Ontario, Canada 
 * moveMotor controls the motor
  @param motor is char A or B refering to motor A or B.
  @param dir is motor direction, CW or CCW
  @speed is PWM value between 0 to 255

  Example 1: to start moving motor A in CW direction with 135 PWM value
  moveMotor('A', CW, 135);

  Example 2: to start moving motor B in CCW direction with 200 PWM value
  moveMotor('B', CCW, 200);  
 */

void moveMotor(char motor, int dir, int speed)
{
  int motorPin;
  int motorSpeedPin;
  
  if(motor =='A')
  {
    motorPin      = MotorPinA;
    motorSpeedPin = MotorSpeedPinA;  
  }else{
    motorPin      = MotorPinB;
    motorSpeedPin = MotorSpeedPinB;     
  }
   digitalWrite(motorPin, dir);// set direction for motor
   analogWrite(motorSpeedPin, speed);// set speed of motor   
}//moveMotor end

/*
 * for function is to be used with DFRobot Motor Shield
 * brake, stops the motor
 * @param motor is character A or B
 * example of usage:
 * brake('A');// stops motor A
 * brake('B');// stops motor B
 */
void brake(char motor)
{
  if(motor =='A')
  {
    digitalWrite(MotorSpeedPinA, 0);// stop motor A
    delay(1000);
  }else{
    digitalWrite(MotorSpeedPinB, 0);// stop motor B
    delay(1000);
   
  }
}// brake end

void rightEncoderEvent() {
  if (digitalRead(RH_ENCODER_A) == HIGH) {
    if (digitalRead(RH_ENCODER_B) == LOW) {
      rightCount++;
    } else {
      rightCount--;
    }
  } else {
    if (digitalRead(RH_ENCODER_B) == LOW) {
      rightCount--;
    } else {
      rightCount++;
    }
  }
}

void leftEncoderEvent() {
  if (digitalRead(LH_ENCODER_A) == HIGH) {
    if (digitalRead(LH_ENCODER_B) == LOW) {
      leftCount++;
    } else {
      leftCount--;
    }
  } else {
    if (digitalRead(LH_ENCODER_B) == LOW) {
      leftCount--;
    } else {
      leftCount++;
    }
  }
}
