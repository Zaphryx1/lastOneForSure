
#include <msp430.h>
#include "libTimer.h"

#define BUZZER BIT6

static const unsigned short melody[] = {440, 494, 523, 587};
static const unsigned short durations[] = {50, 50, 50, 50}; // Durations in 10ms units
static unsigned short currentNote = 0;
static unsigned short noteDuration = 0;

void buzzer_init() {
    P2DIR |= BUZZER;
    P2SEL |= BUZZER;
    buzzer_set_period(0); // Ensure buzzer is off
}

void buzzer_set_period(short cycles) {
    CCR0 = cycles;
    CCR1 = cycles >> 1;
}

void play_tune() {
    if (currentNote < sizeof(melody) / sizeof(melody[0])) {
        buzzer_set_period(2000000 / melody[currentNote]); // Set note frequency
        noteDuration = durations[currentNote]; // Set note duration
        currentNote++;
    } else {
        buzzer_set_period(0); // Stop the buzzer after the tune ends
        currentNote = 0;      // Reset melody
    }
}

void handle_buzzer_timer() {
    if (noteDuration > 0) {
        noteDuration--; // Decrease note duration
    } else if (noteDuration == 0 && currentNote < sizeof(melody) / sizeof(melody[0])) {
        play_tune(); // Move to the next note
    }
}
