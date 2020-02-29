
#include "gpioIn.h"
#include "gpioOut.h"

#define GENOS_GPIO_IN_BOARD_VERSION "1.02"

#define MIDI_CHANNEL 0
#define MIDI_NOTEOFF 0x80
#define MIDI_NOTEON 0x90
#define MIDI_DEFAULT_VELOCITY 0x7f

#define GENOS_FINGERED_ON_BASS_NOTE 42

#define MIDI_OUT_DRV Serial
#define CONSOLE_OUT_DEV Serial
#define DEBUG_ON_UART

GPIO_IN pedal1     = GPIO_IN(2, GPIO_IN_LOGIC_AUTO);
GPIO_IN pedal2     = GPIO_IN(3, GPIO_IN_LOGIC_AUTO);
GPIO_IN pedal3     = GPIO_IN(4, GPIO_IN_LOGIC_AUTO);
GPIO_IN pedalSlow  = GPIO_IN(5);
GPIO_IN pedalFast  = GPIO_IN(6);
GPIO_OUT relay1    = GPIO_OUT(7, GPIO_OUT_LOGIC_REVERTED);

const char PROGMEM title[]   = "genosPedalBoard   v. " GENOS_GPIO_IN_BOARD_VERSION;
const char PROGMEM splash1[] = "Pedal             v. " GPIO_IN_VERSION;
const char PROGMEM splash2[] = "Output            v. " GPIO_OUT_VERSION;
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
    MIDI_OUT_DRV.write(MIDI_NOTEON | GPIO_IN_BOARD_MIDI_CHANNEL);
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
    MIDI_OUT_DRV.write(MIDI_NOTEON | GPIO_IN_BOARD_MIDI_CHANNEL);
    MIDI_OUT_DRV.write(note);
    MIDI_OUT_DRV.write(MIDI_DEFAULT_VELOCITY);
    #endif
}

void pedal1On()
{
    #ifdef DEBUG_ON_UART
    CONSOLE_OUT_DEV.println(F("pedal 1 ON"));
    #endif
}

void pedal2On()
{
    #ifdef DEBUG_ON_UART
    CONSOLE_OUT_DEV.println(F("pedal 2 ON"));
    #endif
}

void pedal3On()
{
    #ifdef DEBUG_ON_UART
    CONSOLE_OUT_DEV.println(F("pedal 3 ON"));
    #endif
}

void pedalSlowOn()
{
    #ifdef DEBUG_ON_UART
    CONSOLE_OUT_DEV.println(F("pedal slow ON"));
    #endif
}

void pedalFastOn()
{
    #ifdef DEBUG_ON_UART
    CONSOLE_OUT_DEV.println(F("pedal fast ON"));
    #endif
}

void pedal1Off()
{
    #ifdef DEBUG_ON_UART
    CONSOLE_OUT_DEV.println(F("pedal 1 OFF"));
    #endif
}

void pedal2Off()
{
    #ifdef DEBUG_ON_UART
    CONSOLE_OUT_DEV.println(F("pedal 2 OFF"));
    #endif
}

void pedal3Off()
{
    #ifdef DEBUG_ON_UART
    CONSOLE_OUT_DEV.println(F("pedal 3 OFF"));
    #endif
}

void pedalSlowOff()
{
    #ifdef DEBUG_ON_UART
    CONSOLE_OUT_DEV.println(F("pedal slow OFF"));
    #endif
}

void pedalFastOff()
{
    #ifdef DEBUG_ON_UART
    CONSOLE_OUT_DEV.println(F("pedal fast OFF"));
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
    cPrintLn(timestamp);
    #else
    CONSOLE_OUT_DEV.begin(31250);
    #endif

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
    relay1.sequencer();

    if(millis() & 0x200)
        digitalWrite(LED_BUILTIN, 0);
    else
        digitalWrite(LED_BUILTIN, 1);
}

