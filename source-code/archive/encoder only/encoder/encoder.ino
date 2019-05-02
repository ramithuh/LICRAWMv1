#include <Encoder.h>


// Change these two numbers to the pins connected to your encoder.
//   Best Performance: both pins have interrupt capability
//   Good Performance: only the first pin has interrupt capability
//   Low Performance:  neither pin has interrupt capability
Encoder myEnc(24, 25);
//   avoid using pins with LEDs attached


void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  Serial.println("Basic Encoder Test:");
}
long oldPosition  = -999;

void loop() {
  // put your main code here, to run repeatedly:


  long newPosition = myEnc.read();
  if (newPosition != oldPosition) {
    oldPosition = newPosition;
    Serial.println(newPosition);
  }
}
