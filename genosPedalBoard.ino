
#include "Pedal.h"
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

PEDAL genericPurpose     = PEDAL(2);
PEDAL fingeredOnBass     = PEDAL(3);
PEDAL sustain            = PEDAL(4);
PEDAL halfMoon           = PEDAL(5);
JACK_OUTPUT genosPedal2  = JACK_OUTPUT(6);
JACK_OUTPUT_PULSE leslie = JACK_OUTPUT_PULSE(7);

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

void pushedGenericPurpose()
{
    #ifdef DEBUG_ON_UART
    CONSOLE_OUT_DEV.println(F("Generic Purpose Pushed"));
    #else
    genosPedal2.switchOn();
    #endif
}

void releasedGenericPurpose()
{
    #ifdef DEBUG_ON_UART
    CONSOLE_OUT_DEV.println(F("Generic Purpose Released"));
    #else
    genosPedal2.switchOff();
    #endif
}

void pushedFingeredOnBass()
{
    noteOn(GENOS_FINGERED_ON_BASS_NOTE);
    noteOff(GENOS_FINGERED_ON_BASS_NOTE);
}

void releasedFingeredOnBass()
{
    noteOn(GENOS_FINGERED_ON_BASS_NOTE);
    noteOff(GENOS_FINGERED_ON_BASS_NOTE);
}

void pushedSustain()
{
    #ifdef DEBUG_ON_UART
    CONSOLE_OUT_DEV.println(F("Sustain switched on (not implemented yet)"));
    #else
    #endif
}

void releasedSustain()
{
    #ifdef DEBUG_ON_UART
    CONSOLE_OUT_DEV.println(F("Sustain switched off (not implemented yet)"));
    #else
    #endif
}

void pushedHalfMoon()
{
    #ifdef DEBUG_ON_UART
    CONSOLE_OUT_DEV.println(F("Half moon switched on"));
    #else
    leslie.sendPulse();
    #endif
}

void releasedHalfMoon()
{
    #ifdef DEBUG_ON_UART
    CONSOLE_OUT_DEV.println(F("Half moon switched off"));
    #else
    leslie.sendPulse();
    #endif
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

    genericPurpose.begin();
    fingeredOnBass.begin();
    sustain.begin();
    halfMoon.begin();
    leslie.begin();
    genosPedal2.begin();

    genericPurpose.setPushedHandler(&pushedGenericPurpose);
    genericPurpose.setReleasedHandler(&releasedGenericPurpose);
    fingeredOnBass.setPushedHandler(&pushedFingeredOnBass);
    fingeredOnBass.setReleasedHandler(&releasedFingeredOnBass);
    sustain.setPushedHandler(&pushedSustain);
    sustain.setReleasedHandler(&releasedSustain);
    halfMoon.setPushedHandler(&pushedHalfMoon);
    halfMoon.setReleasedHandler(&releasedHalfMoon);
    
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
    genericPurpose.sequencer();
    fingeredOnBass.sequencer();
    sustain.sequencer();
    halfMoon.sequencer();
    leslie.sequencer();
    genosPedal2.sequencer();

    if(millis() & 0x200)
        digitalWrite(LED_BUILTIN, 0);
    else
        digitalWrite(LED_BUILTIN, 1);
}

