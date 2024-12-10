
#include <msp430.h>
#include "libTimer.h"

#define BUZZER BIT6

// Melody for "Ahora Te Puedes Marchar"
#define SONG_LENGTH 8
static const unsigned short melody[SONG_LENGTH] = {440, 494, 523, 587, 659, 698, 523, 440}; // Frequencies
static const unsigned short durations[SONG_LENGTH] = {500, 500, 500, 500, 1000, 1000, 500, 250}; // Durations in ms

static unsigned short currentNote = 0;
static unsigned short noteDuration = 0;

void buzzer_init() {
    P2DIR |= BUZZER;
    P2SEL |= BUZZER;
}

void buzzer_set_period(short cycles) {
    CCR0 = cycles;
    CCR1 = cycles >> 1;
}

void play_next_note() {
    if (currentNote < SONG_LENGTH) {
        buzzer_set_period(2000000 / melody[currentNote]);
        noteDuration = durations[currentNote];
        currentNote++;
    } else {
        buzzer_set_period(0); // Stop the buzzer after the song ends
        currentNote = 0;      // Reset to the beginning
    }
}

void start_song() {
    currentNote = 0;
    play_next_note();
}
