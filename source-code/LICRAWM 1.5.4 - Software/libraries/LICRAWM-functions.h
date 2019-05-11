class led{
    private:
        int _pin;

    public:
        led(int pin){                    //Initialize LED
            _pin=pin;
            pinMode(_pin, OUTPUT);
        }

        void on(){
            digitalWrite(_pin, HIGH);   // turn the LED on (HIGH is the voltage level)
        }

        void off(){
            digitalWrite(_pin, LOW);   // turn the LED off (LOW is the voltage level)
        }

        void blink(int miliseconds){  //blink the LED
            digitalWrite(_pin, HIGH);
            delay(miliseconds);
            digitalWrite(_pin, LOW);
            delay(miliseconds);
        }
};

