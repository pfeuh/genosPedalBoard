
#ifndef jackOutputPulse_h
#define jackOutputPulse_h

#include <Arduino.h>

#define JACK_OUTPUT_PULSE_VERSION "1.00"

#define JACK_OUTPUT_PULSE_DURATION 100

class JACK_OUTPUT_PULSE
{
    public:
        JACK_OUTPUT_PULSE(int _pin);
        void begin();
        void sendPulse();
        void sequencer();

    private:
        int pin;
        unsigned long milestone;
        byte state;
        byte nbWaitingEvents;
    
        void setMilestone();
        void setStateOn();
        void setStateOff();
        void setStateSleep();
};

#endif

