/* Contains LICRAWM PIN DEFINITIONS & CLASS DEFINITIONS */
//#define DEBUG true
int default_m1_speed=180;
int default_m2_speed=210;  ///never change or overwrite!! 

int m1_global_speed=180; //150
int m2_global_speed=210; //180

int m1_wall_follow = 180;
int m2_wall_follow = 210;

bool flag=0;
int flag_count = 0;

bool kp_mode=false;
bool ki_mode=false;
bool kd_mode=false;

bool DEBUG_TOF=0;
bool DEBUG_GYRO=0;
bool DEBUG_ENCODERS=0;
bool DEBUG_BLUETOOTH=1;  //turn off during competition
bool DEBUG_SPEED=0;

bool UPDATE_GYRO=0;
bool UPDATE_TOF=1;
bool VISUALIZE =0;

bool FOLLOW_WALL=0;
float tof_error=0;
float offset_distance=80;      //keep this distance from the wall

bool WATER_TRANSFER=0;
bool FOLLOW_LINE=1;
#define SOLVE_MAZE 0

int coin_colour=2;

int no_of_sensors=15;
float KP = 28;   //80     /*was 20*/               //set Kp accordingly
float KI =0;// 3;//was 3                       //set Ki accordingly
float KD = 100;//500;//300;         //240              //set Kd accordingly
float KW = 46;
int mid_val = 1000*(no_of_sensors-1)/2; //position of the middle of the line
int last_error = 0;
float _last_position=0;

#define WRITE_EVERY_MS 20

#define LED_1 23
#define LED_2 25
#define LED_3 27
#define LED_4 29
#define LED_5 31

#define LEFT_TRACKER 48
#define RIGHT_TRACKER 43

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

#define Sensor1_newAddress 50
#define Sensor2_newAddress 43
#define Sensor3_newAddress 44
#define Sensor4_newAddress 45
#define Sensor5_newAddress 46

#define M1_ENCODER_A 2  //encoder pins MOTORB
#define M1_ENCODER_B 3

#define M2_ENCODER_A 19
#define M2_ENCODER_B 18

#define LINE_ARRAY_EVEN_EMITTER_PIN 41
#define LINE_ARRAY_ODD_EMITTER_PIN 42


//volatile
volatile int M2count = 0; //motor encoder counts
volatile int M1count = 0; //motor encoder counts


#define arm_servo_pin 8        //pin for water arm
#define coin_servo_pin 6       //pin for the coin collecting servo


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