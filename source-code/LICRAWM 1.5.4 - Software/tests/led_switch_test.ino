/* Author: Ramith (im@ramith.fyi)

This code is a sample to play with LEDs and Main switches of LICRAWM 1.5.4
there are two classes _led and _switch and we're making instances of them

you can easily check the reading of a switch by using S4.status() command
and LED.write(bool arg) can be used to send a boolean value to write to the LED

*/

#include "../libraries/LICRAWM-io.h"        //has pin definitions
#include "../libraries/LICRAWM-functions.h" //has classes, functions

_led LED5(LED_5);
_led LED1(LED_1);
_led LED3(LED_3);
_led LED2(LED_2);
_led LED4(LED_4);
_switch S2(S_2);
_switch S3(S_3);
_switch S4(S_4);
void setup() {
  
}

void loop() {



  LED4.write(S2.status()); //gets the status of switch2 and writes it to LED4
  LED3.write(S3.status()); //gets the status of switch3 and writes it to LED3
  LED5.write(S4.status()); //gets the status of switch4 and writes it to LED5


}