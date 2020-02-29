#ifndef GPIO_IN_h
#define GPIO_IN_h

#include <Arduino.h>

#define GPIO_IN_VERSION "1.02"

#define GPIO_IN_LOGIC_NORMAL 0
#define GPIO_IN_LOGIC_REVERTED 1
#define GPIO_IN_LOGIC_AUTO 2

typedef void (*GPIO_IN_CB)();

class GPIO_IN
{
    public:
        GPIO_IN(int _pin);
        GPIO_IN(int _pin, byte _logic);
        void setPushedHandler(GPIO_IN_CB handler);
        void setReleasedHandler(GPIO_IN_CB handler);
        void sequencer();

    private:
        int pin;
        bool logic;
        byte previousState;
        GPIO_IN_CB pushedHandler;
        GPIO_IN_CB releasedHandler;
};

#endif

