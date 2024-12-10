
#include <msp430.h>
#include "lcdutils.h"
#include "lcddraw.h"
#include "buzzer.h"
#include "mr_game_watch.h"

// Game state variables
unsigned char position = 1;       // Character position (0, 1, 2)
unsigned char fallingObject = 0;  // Falling object position
unsigned char score = 0;          // Player score
unsigned char collision = 0;      // Collision flag
unsigned char paused = 0;         // Pause flag
unsigned char gameStarted = 0;    // Game start flag

// Function prototypes
void setup();
void moveCharacter(unsigned char direction);
void checkCollision();
void showStartScreen();
void drawGameScreen();
void togglePause();

void main() {
    WDTCTL = WDTPW | WDTHOLD; // Stop watchdog timer
    setup();

    while (!gameStarted) {
        showStartScreen();
        __bis_SR_register(LPM0_bits + GIE); // Enter low-power mode until interrupt
    }

    while (1) {
        if (paused) {
            __bis_SR_register(LPM0_bits + GIE); // Remain in low-power mode while paused
            continue;
        }

        if (collision) {
            P1OUT |= LED_RED;  // Turn on red LED for collision
        } else {
            P1OUT &= ~LED_RED;
            P1OUT |= LED_GREEN;  // Turn on green LED for successful dodge
        }

        drawGameScreen(); // Render the game screen
        __bis_SR_register(LPM0_bits + GIE); // Enter low-power mode until interrupt
    }
}

void setup() {
    configureClocks();
    lcd_init();
    buzzer_init();
    P1DIR |= LED_RED | LED_GREEN;  // Set LEDs as output
    P1OUT &= ~(LED_RED | LED_GREEN);  // Ensure LEDs are off
    enableWDTInterrupts();  // Enable watchdog timer for timing
    initButtons();  // Initialize button interrupts
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

void togglePause() {
    paused = !paused;
    if (paused) {
        P1OUT &= ~(LED_RED | LED_GREEN);  // Turn off LEDs
        buzzer_set_period(0);  // Stop buzzer
    }
}
