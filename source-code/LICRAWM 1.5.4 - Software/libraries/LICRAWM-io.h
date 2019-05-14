/* Contains LICRAWM PIN DEFINITIONS & CLASS DEFINITIONS */
//#define DEBUG true

bool DEBUG_TOF=1;
bool DEBUG_GYRO=1;
bool DEBUG_BLUETOOTH=1;
bool DEBUG_SPEED=0;

bool UPDATE_GYRO=1;
bool UPDATE_TOF=1;

#define WRITE_EVERY_MS 20

#define LED_1 23
#define LED_2 25
#define LED_3 27
#define LED_4 29
#define LED_5 31

#define RESET_PIN 33
#define S_2 22
#define S_3 24
#define S_4 26

#define openmv_p7 30
#define openmv_p8 32
#define openmv_p9 34

#define XSHUT_pin1 45           //Sensor #1's 
#define XSHUT_pin2 47           //Sensor #2's 
#define XSHUT_pin3 49           //Sensor #3        
#define XSHUT_pin4 51
#define XSHUT_pin5 53

#define Sensor1_newAddress 42
#define Sensor2_newAddress 43
#define Sensor3_newAddress 44
#define Sensor4_newAddress 45
#define Sensor5_newAddress 46


class _led{
    private:
        int _pin;

    public:
        _led(int pin){                    //Initialize LED
            _pin=pin;
            pinMode(_pin, OUTPUT);
        }

        void on(int miliseconds=0){
            digitalWrite(_pin, HIGH);   // turn the LED on (HIGH is the voltage level)
            delay(miliseconds);
        }

        void off(int miliseconds=0){
            digitalWrite(_pin, LOW);   // turn the LED off (LOW is the voltage level)
            delay(miliseconds);
        }
        void toggle(){
            digitalWrite(_pin, !digitalRead(_pin));
        }
        void blink(int miliseconds){  //blink the LED
            digitalWrite(_pin, HIGH);
            delay(miliseconds);
            digitalWrite(_pin, LOW);
            delay(miliseconds);
        }
        void write(bool status){
            digitalWrite(_pin,status);
        }
};

class _switch{
    private:
        int _pin;

    public:
        _switch(int pin){                    //Initialize switch
            _pin=pin;
            pinMode(_pin, INPUT); 
        }

        bool on(){
            return digitalRead(_pin);
        }

        bool off(){
            return !digitalRead(_pin);
        }
        bool status(){
            return digitalRead(_pin);
        }
};