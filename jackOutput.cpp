
#include "jackOutput.h"

JACK_OUTPUT::JACK_OUTPUT(int _pin)
{
    pin = _pin;
}

void JACK_OUTPUT::begin()
{
    pinMode(pin, OUTPUT);
    switchOff();
    
}

void JACK_OUTPUT::switchOn()
{
    digitalWrite(pin, 1);
}

void JACK_OUTPUT::switchOff()
{
    digitalWrite(pin, 0);
}

void JACK_OUTPUT::sequencer()
{
}


