
#include "gpioOut.h"

/*******************/
/* Private methods */
/*******************/

void GPIO_OUT::init()
{
    running = 0;
    pinMode(pin, OUTPUT);
    off();
}

/******************/
/* Public methods */
/******************/

GPIO_OUT::GPIO_OUT(int _pin)
{
    pin = _pin;
    logic = GPIO_OUT_LOGIC_NORMAL;
    init();
}

GPIO_OUT::GPIO_OUT(int _pin, bool _logic)
{
    pin = _pin;
    logic = _logic;
    init();
}

void GPIO_OUT::on()
{
    state = logic;
    digitalWrite(pin, state);
}

void GPIO_OUT::off()
{
    state = !logic;
    digitalWrite(pin, state);
}

void GPIO_OUT::toggle()
{
    state ^= 1;
    digitalWrite(pin, state);
}

void GPIO_OUT::pulse(unsigned long duration)
{
    milestone = millis() + duration;
    running = 1;
    on();
}

void GPIO_OUT::sequencer()
{
    if(running)
    {
        if(millis() >= milestone)
        {
            running = 0;
            off();
        }
    }
}

