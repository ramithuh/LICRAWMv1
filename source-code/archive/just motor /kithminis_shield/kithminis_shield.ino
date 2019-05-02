#include<AFMotor.h>

AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
void setup() {
  // put your setup code here, to run once:
  motor1.setSpeed(135);
  motor2.setSpeed(185);

}

void loop() {
  // put your main code here, to run repeatedly:

  motor1.run(FORWARD);
  motor2.run(FORWARD);
}
