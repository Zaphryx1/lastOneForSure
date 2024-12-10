
#include <msp430.h>
#include "lcdutils.h"
#include "lcddraw.h"
#include "mr_game_watch.h"
#include "buzzer.h"

// State variables
unsigned char position = 1;       // Character position (0, 1, 2)
unsigned int score = 0;           // Player score
unsigned char fallingObject = 0;  // Falling object position
unsigned char collision = 0;      // Collision flag
unsigned char gameStarted = 0;    // Flag to indicate if the game has started

// Function prototypes
void setup();
void startTune();
void moveCharacter(unsigned char direction);
void checkCollision();
void showStartScreen();
void drawGameScreen();

void main() {
    WDTCTL = WDTPW | WDTHOLD; // Stop watchdog timer
    setup();

    while (!gameStarted) {
        showStartScreen();
        __bis_SR_register(LPM0_bits + GIE); // Wait for button press (low power mode)
    }

    startTune(); // Play music when the game starts

    while (1) {
        if (collision) {
            P1OUT |= LED_RED; // Red LED indicates collision
        } else {
            P1OUT &= ~LED_RED;
            P1OUT |= LED_GREEN; // Green LED indicates successful dodge
        }

        drawGameScreen(); // Render the game

        __bis_SR_register(LPM0_bits + GIE); // Enter low power mode with interrupts enabled
    }
}

void setup() {
    configureClocks();
    lcd_init();
    buzzer_init();
    P1DIR |= LED_RED | LED_GREEN; // Set LEDs as output
    P1OUT &= ~(LED_RED | LED_GREEN); // Ensure LEDs are off
    enableWDTInterrupts(); // Enable watchdog timer for game timing
}

void startTune() {
    // Play the tune "Ahora Te Puedes Marchar"
    const int melody[] = {440, 494, 523, 587, 659, 698, 523, 440}; // Example frequencies
    const int duration[] = {500, 500, 500, 500, 1000, 1000, 500, 250}; // Durations in ms

    for (int i = 0; i < 8; i++) {
        buzzer_set_period(2000000 / melody[i]);
        __delay_cycles(duration[i] * 1000);
    }
    buzzer_set_period(0); // Turn off buzzer
}

void moveCharacter(unsigned char direction) {
    if (direction == 1 && position > 0) {
        position--; // Move left
    } else if (direction == 2 && position < 2) {
        position++; // Move right
    }
}

void checkCollision() {
    if (fallingObject == position) {
        collision = 1; // Collision occurred
    } else {
        collision = 0; // No collision
        score++;
    }
}

void showStartScreen() {
    clearScreen(COLOR_BLACK);
    drawString5x7(20, 60, "Press any button", COLOR_WHITE, COLOR_BLACK);
    drawString5x7(20, 80, "to start!", COLOR_WHITE, COLOR_BLACK);
}

void drawGameScreen() {
    clearScreen(COLOR_BLACK);

    // Draw the character
    unsigned int charX = position * 40 + 20;
    fillRectangle(charX, 100, 20, 20, COLOR_WHITE);

    // Draw the falling object
    unsigned int objX = fallingObject * 40 + 20;
    fillRectangle(objX, 20, 10, 10, COLOR_RED);
}
