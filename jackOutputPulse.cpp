
#include "jackOutputPulse.h"

#define JACK_OUTPUT_PULSE_STATE_SLEEP 0
#define JACK_OUTPUT_PULSE_STATE_ON 1
#define JACK_OUTPUT_PULSE_STATE_OFF 2
#define JACK_OUTPUT_PULSE_OFF 1
#define JACK_OUTPUT_PULSE_ON 0

/*******************/
/* Private methods */
/*******************/
void JACK_OUTPUT_PULSE::setMilestone()
{
    milestone = millis() + JACK_OUTPUT_PULSE_DURATION;
}

void JACK_OUTPUT_PULSE::setStateOn()
{
    nbWaitingEvents--;
    digitalWrite(pin, JACK_OUTPUT_PULSE_ON);
    state = JACK_OUTPUT_PULSE_STATE_ON;
    setMilestone();
}

void JACK_OUTPUT_PULSE::setStateOff()
{
    digitalWrite(pin, JACK_OUTPUT_PULSE_OFF);
    state = JACK_OUTPUT_PULSE_STATE_OFF;
    setMilestone();
}

void JACK_OUTPUT_PULSE::setStateSleep()
{
    digitalWrite(pin, JACK_OUTPUT_PULSE_OFF);
    state = JACK_OUTPUT_PULSE_STATE_OFF;
}

/******************/
/* Public methods */
/******************/

JACK_OUTPUT_PULSE::JACK_OUTPUT_PULSE(int _pin)
{
    pin = _pin;
}

void JACK_OUTPUT_PULSE::begin()
{
    pinMode(pin, OUTPUT);
    setStateSleep();
}

void JACK_OUTPUT_PULSE::sendPulse()
{
    nbWaitingEvents++;
}

void JACK_OUTPUT_PULSE::sequencer()
{
    if (state == JACK_OUTPUT_PULSE_STATE_SLEEP)
    switch(state)
    {
        case JACK_OUTPUT_PULSE_STATE_SLEEP:
            if(nbWaitingEvents)
                setStateOn();
            break;
        case JACK_OUTPUT_PULSE_STATE_ON:
            if(millis() >= milestone)
                setStateOff();
            break;
        case JACK_OUTPUT_PULSE_STATE_OFF:
            if(millis() >= milestone)
                setStateSleep();
            break;
    }
}


