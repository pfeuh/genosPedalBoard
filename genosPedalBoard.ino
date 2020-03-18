
#include "gpioIn.h"
#include "gpioOut.h"

#define GENOS_GPIO_IN_BOARD_VERSION "1.02"

#define MIDI_CHANNEL 0
#define MIDI_NOTEOFF 0x80
#define MIDI_NOTEON 0x90
#define MIDI_CC 0xb0
#define MIDI_DEFAULT_VELOCITY 0x7f
#define MIDI_CC_VALUE_OFF 0
#define MIDI_CC_VALUE_ON 127

#define GENOS_FINGERED_ON_BASS_CC 34
#define SUSTAIN_CC 1
#define PORTAMENTO_CC 2

/* custom config */
#define PEDAL_1_CC GENOS_FINGERED_ON_BASS_CC
#define PEDAL_2_CC SUSTAIN_CC
#define PEDAL_3_CC PORTAMENTO_CC

#define MIDI_OUT_DRV Serial
#define CONSOLE_OUT_DEV Serial
//~ #define DEBUG_ON_UART

GPIO_IN pedal_1      = GPIO_IN(2, GPIO_IN_LOGIC_AUTO);
GPIO_IN pedal_2      = GPIO_IN(3, GPIO_IN_LOGIC_AUTO);
GPIO_IN pedal_3      = GPIO_IN(4, GPIO_IN_LOGIC_AUTO);
GPIO_IN unusedPedal  = GPIO_IN(5);
GPIO_IN sustainPedal = GPIO_IN(6);
GPIO_OUT relay1      = GPIO_OUT(7, GPIO_OUT_LOGIC_REVERTED);

const char PROGMEM title[]   = "genosPedalBoard   v. " GENOS_GPIO_IN_BOARD_VERSION;
const char PROGMEM splash1[] = "Pedal             v. " GPIO_IN_VERSION;
const char PROGMEM splash2[] = "Output            v. " GPIO_OUT_VERSION;
const char PROGMEM timestamp[] = __DATE__ " " __TIME__;

void cPrint(const char* address)
{
    while(pgm_read_byte(address))
        CONSOLE_OUT_DEV.write(pgm_read_byte(address++));
}

void cPrintLn(const char* address)
{
    cPrint(address);
    CONSOLE_OUT_DEV.write('\n');
}

void noteOn(byte note)
{
    #ifdef DEBUG_ON_UART
    CONSOLE_OUT_DEV.print(MIDI_NOTEON | MIDI_CHANNEL, HEX);
    CONSOLE_OUT_DEV.write(' ');
    CONSOLE_OUT_DEV.print(note, HEX);
    CONSOLE_OUT_DEV.write(' ');
    CONSOLE_OUT_DEV.println(MIDI_DEFAULT_VELOCITY, HEX);
    #else
    MIDI_OUT_DRV.write(MIDI_NOTEON | MIDI_CHANNEL);
    MIDI_OUT_DRV.write(note);
    MIDI_OUT_DRV.write(MIDI_DEFAULT_VELOCITY);
    #endif
}

void noteOff(byte note)
{
    #ifdef DEBUG_ON_UART
    CONSOLE_OUT_DEV.print(MIDI_NOTEOFF | MIDI_CHANNEL, HEX);
    CONSOLE_OUT_DEV.write(' ');
    CONSOLE_OUT_DEV.print(note);
    CONSOLE_OUT_DEV.write(' ');
    CONSOLE_OUT_DEV.println(0);
    #else
    MIDI_OUT_DRV.write(MIDI_NOTEOFF | MIDI_CHANNEL);
    MIDI_OUT_DRV.write(note);
    MIDI_OUT_DRV.write(MIDI_DEFAULT_VELOCITY);
    #endif
}

void controlChange(byte index, byte value)
{
    #ifdef DEBUG_ON_UART
    CONSOLE_OUT_DEV.print(MIDI_CC | MIDI_CHANNEL, HEX);
    CONSOLE_OUT_DEV.write(' ');
    CONSOLE_OUT_DEV.print(index, HEX);
    CONSOLE_OUT_DEV.write(' ');
    CONSOLE_OUT_DEV.println(value, HEX);
    #else
    MIDI_OUT_DRV.write(MIDI_CC | MIDI_CHANNEL);
    MIDI_OUT_DRV.write(index);
    MIDI_OUT_DRV.write(value);
    #endif
}

void pedal1_On()
{
    controlChange(PEDAL_1_CC, MIDI_CC_VALUE_ON);
    controlChange(PEDAL_1_CC, MIDI_CC_VALUE_OFF);
}

void pedal2_On()
{
    controlChange(PEDAL_2_CC, MIDI_CC_VALUE_ON);
    //~ relay1.on();
}

void pedal3_On()
{
    controlChange(PEDAL_3_CC, MIDI_CC_VALUE_ON);
}

void unusedPedalOn()
{
    #ifdef DEBUG_ON_UART
    CONSOLE_OUT_DEV.println(F("pedal slow ON"));
    #endif
}

void sustainPedalOn()
{
    #ifdef DEBUG_ON_UART
    CONSOLE_OUT_DEV.println(F("pedal fast ON"));
    #endif
}

void pedal1_Off()
{
    controlChange(PEDAL_1_CC, MIDI_CC_VALUE_ON);
    controlChange(PEDAL_1_CC, MIDI_CC_VALUE_OFF);
}

void pedal2_Off()
{
    controlChange(PEDAL_2_CC, MIDI_CC_VALUE_OFF);
    //~ relay1.off();
}

void pedal3_Off()
{
    controlChange(PEDAL_3_CC, MIDI_CC_VALUE_OFF);
}

void unusedPedalOff()
{
}

void sustainPedalOff()
{
}

void setup()
{
    #ifdef DEBUG_ON_UART
    CONSOLE_OUT_DEV.begin(9600);
    cPrintLn(title);
    cPrintLn(splash1);
    cPrintLn(splash2);
    cPrintLn(timestamp);
    #else
    CONSOLE_OUT_DEV.begin(31250);
    #endif

    pedal_1.setPushedHandler(&pedal1_On);
    pedal_1.setReleasedHandler(&pedal1_Off);
    pedal_2.setPushedHandler(&pedal2_On);
    pedal_2.setReleasedHandler(&pedal2_Off);
    pedal_3.setPushedHandler(&pedal3_On);
    pedal_3.setReleasedHandler(&pedal3_Off);
    unusedPedal.setPushedHandler(&unusedPedalOn);
    unusedPedal.setReleasedHandler(&unusedPedalOff);
    sustainPedal.setPushedHandler(&sustainPedalOn);
    sustainPedal.setReleasedHandler(&sustainPedalOff);
    
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
    pedal_1.sequencer();
    pedal_2.sequencer();
    pedal_3.sequencer();
    unusedPedal.sequencer();
    sustainPedal.sequencer();
    relay1.sequencer();

    if(millis() & 0x200)
        digitalWrite(LED_BUILTIN, 0);
    else
        digitalWrite(LED_BUILTIN, 1);
}

