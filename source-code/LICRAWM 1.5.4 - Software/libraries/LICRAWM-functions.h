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

void LED_all_off(int miliseconds=0){
    digitalWrite(LED_1, LOW);   
    digitalWrite(LED_2, LOW); 
    digitalWrite(LED_3, LOW); 
    digitalWrite(LED_4, LOW); 
    digitalWrite(LED_5, LOW); 
    delay(miliseconds);
}

void LED_all_on(int miliseconds=0){
    digitalWrite(LED_1, HIGH);   
    digitalWrite(LED_2, HIGH); 
    digitalWrite(LED_3, HIGH); 
    digitalWrite(LED_4, HIGH); 
    digitalWrite(LED_5, HIGH); 
    delay(miliseconds);
}