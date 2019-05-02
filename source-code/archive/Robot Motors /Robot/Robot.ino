#include <Encoder.h>


// Change these two numbers to the pins connected to your encoder.
//   Best Performance: both pins have interrupt capability
//   Good Performance: only the first pin has interrupt capability
//   Low Performance:  neither pin has interrupt capability
Encoder myEnc(24, 25);
//   avoid using pins with LEDs attached


void setup() {
  // put your setup code here, to run once:
  pinMode(5,OUTPUT);  //LM
  pinMode(3,OUTPUT);  //RM
  pinMode(22,OUTPUT); //LM
  pinMode(53,OUTPUT);  //RM
  Serial.begin(9600);
  Serial.println("Basic Encoder Test:");
}
long oldPosition  = -999;

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(22,HIGH); //LM FWD
  digitalWrite(53,LOW); //RM FWD

  analogWrite(5,255);
  analogWrite(3,255);

  long newPosition = myEnc.read();
  if (newPosition != oldPosition) {
    oldPosition = newPosition;
    Serial.println(newPosition);
  }
  /*for(int i =50;i<255;i++)
  {
    analogWrite(2,i);
    analogWrite(3,i);
    delayMicroseconds(50);
  }
  
  for(int j =255;j>=0;j--)
  {
    analogWrite(2,j);
    analogWrite(3,j);
    delayMicroseconds(50);
  }

  digitalWrite(22,HIGH); //LM BWD
  digitalWrite(23,HIGH); //RM BWD
  for(int i =50;i<255;i++)
  {
    analogWrite(2,i);
    analogWrite(3,i);
    delayMicroseconds(50);
  }

  for(int j =255;j>=0;j--)
  {
    analogWrite(2,j);
    analogWrite(3,j);
    delayMicroseconds(50);
  }*/
}
