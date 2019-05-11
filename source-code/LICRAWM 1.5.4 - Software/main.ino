#include "libraries/LICRAWM-io.h"
#include "libraries/LICRAWM-functions.h"

led LED5(LED_5);
led powerled(LED_1);
led statusled(LED_2);

void setup() {
  powerled.on();
}

void loop() {
  statusled.on();
  LED5.blink(200);
 
}

