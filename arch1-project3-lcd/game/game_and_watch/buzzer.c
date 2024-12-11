
#include <msp430.h>
#include "libTimer.h"

#define BUZZER BIT6

static const unsigned short melody[] = {440, 494, 523, 587};
static const unsigned short durations[] = {500, 500, 500, 500};
static unsigned short currentNote = 0;

void buzzer_init() {
    P2DIR |= BUZZER;
    P2SEL |= BUZZER;
}

void buzzer_set_period(short cycles) {
    CCR0 = cycles;
    CCR1 = cycles >> 1;
}

void play_tune() {
    if (currentNote < sizeof(melody) / sizeof(melody[0])) {
        buzzer_set_period(2000000 / melody[currentNote]);
        __delay_cycles(durations[currentNote] * 1000);
        currentNote++;
    } else {
        buzzer_set_period(0); // Stop the buzzer after the tune
        currentNote = 0;      // Reset to the start of the melody
    }
}
