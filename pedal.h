#ifndef pedal_h
#define pedal_h

#include <Arduino.h>

#define PEDAL_VERSION "1.01"

#define PEDAL_LOGIC_NORMAL 0
#define PEDAL_LOGIC_REVERTED 1
#define PEDAL_LOGIC_AUTO 2

typedef void (*pedal_CB)();

class PEDAL
{
    public:
        PEDAL(int _pin);
        PEDAL(int _pin, byte _logic);
        void begin();
        void setPushedHandler(pedal_CB handler);
        void setReleasedHandler(pedal_CB handler);
        void sequencer();

    private:
        int pin;
        //~ bool init_state;
        bool logic;
        byte previousState;
        pedal_CB pushedHandler;
        pedal_CB releasedHandler;
};

#endif

