
#include "pedal.h"

/*******************/
/* Private methods */
/*******************/


/******************/
/* Public methods */
/******************/

PEDAL::PEDAL(int _pin)
{
    pin = _pin;
}

void PEDAL::begin()
{
    pinMode(pin, INPUT_PULLUP);
    previousState = 1;
    pushedHandler = 0;
    releasedHandler = 0;
}

void PEDAL::setPushedHandler(pedal_CB handler)
{
    pushedHandler = handler;
}

void PEDAL::setReleasedHandler(pedal_CB handler)
{
    releasedHandler = handler;
}

void PEDAL::sequencer()
{
    byte new_state = digitalRead(pin);
    if(new_state != previousState)
    {
        if(new_state)
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


