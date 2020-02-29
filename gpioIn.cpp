
#include "gpioIn.h"

/*******************/
/* Private methods */
/*******************/

/******************/
/* Public methods */
/******************/

GPIO_IN::GPIO_IN(int _pin)
{
    pin = _pin;
    logic = GPIO_IN_LOGIC_NORMAL;
    pinMode(pin, INPUT_PULLUP);
    previousState = 1;
    pushedHandler = 0;
    releasedHandler = 0;
}

GPIO_IN::GPIO_IN(int _pin, byte _logic)
{
    pin = _pin;
    pinMode(pin, INPUT_PULLUP);
    bool init_state = digitalRead(pin);
    previousState = init_state;
    switch(_logic)
    {
        case GPIO_IN_LOGIC_NORMAL:
            logic = 0;
            break;
        case GPIO_IN_LOGIC_REVERTED:
            logic = 1;
            break;
        case GPIO_IN_LOGIC_AUTO:
            logic = !init_state;
            break;
    }
    pushedHandler = 0;
    releasedHandler = 0;
}

void GPIO_IN::setPushedHandler(GPIO_IN_CB handler)
{
    pushedHandler = handler;
}

void GPIO_IN::setReleasedHandler(GPIO_IN_CB handler)
{
    releasedHandler = handler;
}

void GPIO_IN::sequencer()
{
    byte new_state = digitalRead(pin);
    if(new_state != previousState)
    {
        if(new_state ^ logic)
        {
            if(releasedHandler)
                releasedHandler();
        }
        else
        {
            if(pushedHandler)
                pushedHandler();
        }
       previousState = new_state;
    }
}


