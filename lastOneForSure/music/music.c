
#include <msp430.h>
#include "libTimer.h"

#define BUZZER BIT6 // Buzzer connected to P2.6

// Melody for "Ahora Te Puedes Marchar"
#define SONG_LENGTH 8
short melody[SONG_LENGTH] = {440, 494, 523, 587, 659, 698, 523, 440}; // Frequencies
short durations[SONG_LENGTH] = {500, 500, 500, 500, 1000, 1000, 500, 250}; // Durations in ms

// Playback state variables
static unsigned short currentNote = 0;
static unsigned short noteDuration = 0;

void buzzer_init() {
    P2DIR |= BUZZER;       // Set buzzer pin as output
    P2SEL |= BUZZER;       // Enable buzzer output
}

void play_next_note() {
    if (currentNote < SONG_LENGTH) {
        if (melody[currentNote] == 0) {
            buzzer_set_period(0); // Silence for pauses
        } else {
            buzzer_set_period(2000000 / melody[currentNote]); // Set frequency
        }
        noteDuration = durations[currentNote];
        currentNote++;
    } else {
        buzzer_set_period(0); // Stop playback
        currentNote = 0;      // Reset to the beginning
    }
}

void buzzer_set_period(short cycles) {
    CCR0 = cycles;      // Set period
    CCR1 = cycles >> 1; // Set duty cycle to 50%
}

void start_song() {
    currentNote = 0;    // Reset song to the beginning
    play_next_note();   // Start the first note
}
