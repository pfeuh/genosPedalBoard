#ifndef pedal_h
#define pedal_h

#include <Arduino.h>

#define PEDAL_VERSION "1.00"

typedef void (*pedal_CB)();

class PEDAL
{
    public:
        PEDAL(int _pin);
        void begin();
        void setPushedHandler(pedal_CB handler);
        void setReleasedHandler(pedal_CB handler);
        void sequencer();

    private:
        int pin;
        byte previousState;
        pedal_CB pushedHandler;
        pedal_CB releasedHandler;
};

#endif

