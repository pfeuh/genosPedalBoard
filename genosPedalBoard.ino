
#include "pedal.h"
#include "jackOutput.h"
#include "jackOutputPulse.h"

// --Function-----------input---------output-----
// A Generic Purpose    PedalBoard    Output Jack
// B Fingered On Bass   pedalBoard    midi
// C Sustain            pedalboard    midi
// D Leslie Half Moon   Input Jack    Output Jack
// -----------------------------------------------

#define GENOS_PEDAL_BOARD_VERSION "1.00"

#define MIDI_CHANNEL 0
#define MIDI_NOTEOFF 0x80
#define MIDI_NOTEON 0x90
#define MIDI_DEFAULT_VELOCITY 0x7f

#define GENOS_FINGERED_ON_BASS_NOTE 42

#define MIDI_OUT_DRV Serial
#define CONSOLE_OUT_DEV Serial
#define DEBUG_ON_UART

PEDAL pedal1     = PEDAL(2, PEDAL_LOGIC_AUTO);
PEDAL pedal2     = PEDAL(3, PEDAL_LOGIC_AUTO);
PEDAL pedal3     = PEDAL(4, PEDAL_LOGIC_AUTO);
PEDAL pedalSlow  = PEDAL(5);
PEDAL pedalFast  = PEDAL(6);
//~ JACK_OUTPUT genosPedal2  = JACK_OUTPUT(7);
//~ JACK_OUTPUT_PULSE leslie = JACK_OUTPUT_PULSE(8);

const char PROGMEM title[]   = "genosPedalBoard   v. " GENOS_PEDAL_BOARD_VERSION;
const char PROGMEM splash1[] = "Pedal             v. " PEDAL_VERSION;
const char PROGMEM splash2[] = "Jack Output       v. " JACK_OUTPUT_VERSION;
const char PROGMEM splash3[] = "Jack Output Pulse v. " JACK_OUTPUT_PULSE_VERSION;
const char PROGMEM timestamp[] = __DATE__ " " __TIME__;

void noteOn(byte note)
{
    #ifdef DEBUG_ON_UART
    CONSOLE_OUT_DEV.print(MIDI_NOTEON | MIDI_CHANNEL, HEX);
    CONSOLE_OUT_DEV.write(' ');
    CONSOLE_OUT_DEV.print(note, HEX);
    CONSOLE_OUT_DEV.write(' ');
    CONSOLE_OUT_DEV.println(MIDI_DEFAULT_VELOCITY, HEX);
    #else
    MIDI_OUT_DRV.write(MIDI_NOTEON | PEDAL_BOARD_MIDI_CHANNEL);
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
    MIDI_OUT_DRV.write(MIDI_NOTEON | PEDAL_BOARD_MIDI_CHANNEL);
    MIDI_OUT_DRV.write(note);
    MIDI_OUT_DRV.write(MIDI_DEFAULT_VELOCITY);
    #endif
}

void pedal1On()
{
    CONSOLE_OUT_DEV.println(F("pedal 1 ON"));
}

void pedal2On()
{
    CONSOLE_OUT_DEV.println(F("pedal 2 ON"));
}

void pedal3On()
{
    CONSOLE_OUT_DEV.println(F("pedal 3 ON"));
}

void pedalSlowOn()
{
    CONSOLE_OUT_DEV.println(F("pedal slow ON"));
}

void pedalFastOn()
{
    CONSOLE_OUT_DEV.println(F("pedal fast ON"));
}

void pedal1Off()
{
    CONSOLE_OUT_DEV.println(F("pedal 1 OFF"));
}

void pedal2Off()
{
    CONSOLE_OUT_DEV.println(F("pedal 2 OFF"));
}

void pedal3Off()
{
    CONSOLE_OUT_DEV.println(F("pedal 3 OFF"));
}

void pedalSlowOff()
{
    CONSOLE_OUT_DEV.println(F("pedal slow OFF"));
}

void pedalFastOff()
{
    CONSOLE_OUT_DEV.println(F("pedal fast OFF"));
}

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

void setup()
{
    #ifdef DEBUG_ON_UART
    CONSOLE_OUT_DEV.begin(9600);
    cPrintLn(title);
    cPrintLn(splash1);
    cPrintLn(splash2);
    cPrintLn(splash3);
    cPrintLn(timestamp);
    #else
    CONSOLE_OUT_DEV.begin(31250);
    #endif

    pedal1.begin();
    pedal2.begin();
    pedal3.begin();
    pedalSlow.begin();
    pedalFast.begin();

    pedal1.setPushedHandler(&pedal1On);
    pedal1.setReleasedHandler(&pedal1Off);
    pedal2.setPushedHandler(&pedal2On);
    pedal2.setReleasedHandler(&pedal2Off);
    pedal3.setPushedHandler(&pedal3On);
    pedal3.setReleasedHandler(&pedal3Off);
    pedalSlow.setPushedHandler(&pedalSlowOn);
    pedalSlow.setReleasedHandler(&pedalSlowOff);
    pedalFast.setPushedHandler(&pedalFastOn);
    pedalFast.setReleasedHandler(&pedalFastOff);
    
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
    pedal1.sequencer();
    pedal2.sequencer();
    pedal3.sequencer();
    pedalSlow.sequencer();
    pedalFast.sequencer();

    if(millis() & 0x200)
        digitalWrite(LED_BUILTIN, 0);
    else
        digitalWrite(LED_BUILTIN, 1);
}

