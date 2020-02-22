
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
    logic = PEDAL_LOGIC_NORMAL;
    pinMode(pin, INPUT_PULLUP);
    previousState = 1;
    pushedHandler = 0;
    releasedHandler = 0;
}

PEDAL::PEDAL(int _pin, byte _logic)
{
    pin = _pin;
    pinMode(pin, INPUT_PULLUP);
    bool init_state = digitalRead(pin);
    previousState = init_state;
    switch(_logic)
    {
        case PEDAL_LOGIC_NORMAL:
            logic = 0;
            break;
        case PEDAL_LOGIC_REVERTED:
            logic = 1;
        case PEDAL_LOGIC_AUTO:
            logic = !init_state;
            break;
    }
    pushedHandler = 0;
    releasedHandler = 0;
}

void PEDAL::begin()
{
    /* just for Arduino compliance */
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


