
#ifndef jackOutput_h
#define jackOutput_h

#include <Arduino.h>

#define JACK_OUTPUT_VERSION "1.00"

class JACK_OUTPUT
{
    public:
        JACK_OUTPUT(int _pin);
        void begin();
        void switchOn();
        void switchOff();
        void sequencer();

    private:
        int pin;
};

#endif

