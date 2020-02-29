#ifndef GPIO_OUT_h
#define GPIO_OUT_h

#include <Arduino.h>

#define GPIO_OUT_VERSION "1.00"

#define GPIO_OUT_LOGIC_NORMAL 1
#define GPIO_OUT_LOGIC_REVERTED 0

typedef void (*GPIO_OUT_CB)();

class GPIO_OUT
{
    public:
        GPIO_OUT(int _pin);
        GPIO_OUT(int _pin, bool _logic);
        void on();
        void off();
        void toggle();
        void pulse(unsigned long duration);
        void sequencer();

    private:
        int pin;
        bool logic;
        bool state;
        unsigned long milestone;
        byte running;
        byte previousState;
    
        void init();
};

#endif

