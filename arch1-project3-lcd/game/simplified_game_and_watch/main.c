
#include <msp430.h>
#include "lcdutils.h"
#include "lcddraw.h"
#include "buzzer.h"

// Game state variables
unsigned char position = 1;       // Character position (0, 1, 2)
unsigned char fallingObject = 0;  // Falling object position
unsigned int score = 0;           // Player score
unsigned char paused = 0;         // Pause flag

// Function prototypes
void setup();
void moveCharacter(unsigned char direction);
void drawStartScreen();
void drawGameScreen();
void checkCollision();

void main() {
    WDTCTL = WDTPW | WDTHOLD; // Stop watchdog timer
    setup();

    while (1) {
        if (paused) {
            drawStartScreen(); // Show start/pause screen
        } else {
            drawGameScreen(); // Render game screen
            checkCollision(); // Check for collisions
        }
        __bis_SR_register(LPM0_bits + GIE); // Enter low-power mode until interrupt
    }
}

void setup() {
    configureClocks();
    lcd_init();
    buzzer_init();
    P1DIR |= LED_RED | LED_GREEN;  // Set LEDs as output
    P1OUT &= ~(LED_RED | LED_GREEN);  // Ensure LEDs are off
    enableWDTInterrupts();  // Enable watchdog timer interrupts
    initButtons();  // Initialize buttons
}

void moveCharacter(unsigned char direction) {
    if (direction == 1 && position > 0) {
        position--; // Move left
    } else if (direction == 2 && position < 2) {
        position++; // Move right
    }
}

void drawStartScreen() {
    clearScreen(COLOR_BLACK);
    drawString5x7(20, 50, "Press S3 to Start", COLOR_WHITE, COLOR_BLACK);
}

void drawGameScreen() {
    clearScreen(COLOR_BLACK);

    // Draw character
    unsigned int charX = position * 40 + 20;
    fillRectangle(charX, 100, 20, 20, COLOR_WHITE);

    // Draw falling object
    unsigned int objX = fallingObject * 40 + 20;
    fillRectangle(objX, 20, 10, 10, COLOR_RED);

    // Display score
    char scoreStr[10];
    sprintf(scoreStr, "Score: %d", score);
    drawString5x7(5, 5, scoreStr, COLOR_WHITE, COLOR_BLACK);
}

void checkCollision() {
    if (fallingObject == position) {
        P1OUT |= LED_RED; // Collision detected, turn on red LED
        paused = 1;       // Pause the game
    } else {
        P1OUT |= LED_GREEN; // Successful dodge, turn on green LED
        score++;            // Increment score
    }
}

void __attribute__((interrupt(WDT_VECTOR))) WDT_ISR() {
    static unsigned char timerCount = 0;
    if (++timerCount == 50) { // Every ~1 second (assuming ~20ms WDT tick)
        fallingObject = (fallingObject + 1) % 3; // Move falling object
        timerCount = 0;
    }
}
