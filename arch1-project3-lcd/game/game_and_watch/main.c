
#include <msp430.h>
#include "lcdutils.h"
#include "lcddraw.h"
#include "buzzer.h"
#include "game_and_watch.h"

// External assembly function
extern unsigned int score_increment(unsigned int);

// Game state variables
unsigned char position = 1;       // Character position (0, 1, 2)
unsigned char fallingObject = 0;  // Falling object position
unsigned char paused = 0;         // Pause flag
unsigned int score = 0;           // Player score

// Function prototypes
void setup();
void moveCharacter(unsigned char direction);
void drawStartScreen();
void drawGameScreen();
void checkCollision();
void timerHandler();

void main() {
    WDTCTL = WDTPW | WDTHOLD; // Stop watchdog timer
    setup();

    while (1) {
        if (!paused) {
            drawGameScreen(); // Render game screen
            checkCollision(); // Check collisions
        } else {
            drawStartScreen(); // Display start or pause screen
        }
        __bis_SR_register(LPM0_bits + GIE); // Enter low-power mode until interrupt
    }
}

void setup() {
    configureClocks();
    lcd_init();
    buzzer_init();
    P1DIR |= LED_RED | LED_GREEN; // Set LEDs as output
    P1OUT &= ~(LED_RED | LED_GREEN); // Ensure LEDs are off
    enableWDTInterrupts(); // Enable watchdog timer interrupts
    initButtons(); // Initialize buttons
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
    drawString8x12(20, 50, "Press S3 to", COLOR_WHITE, COLOR_BLACK);
    drawString8x12(20, 70, "Start/Pause", COLOR_WHITE, COLOR_BLACK);
}

void drawGameScreen() {
    clearScreen(COLOR_BLACK);

    // Draw character
    unsigned int charX = position * 40 + 20;
    fillRectangle(charX, 100, 20, 20, COLOR_WHITE);

    // Draw falling object
    unsigned int objX = fallingObject * 40 + 20;
    fillRectangle(objX, 20, 10, 10, COLOR_RED);

    // Update score using the assembly function
    score = score_increment(score);

    // Display score
    char scoreStr[10];
    sprintf(scoreStr, "Score: %d", score);
    drawString8x12(5, 5, scoreStr, COLOR_WHITE, COLOR_BLACK);
}

void checkCollision() {
    if (fallingObject == position) {
        P1OUT |= LED_RED; // Turn on red LED for collision
        paused = 1;       // Pause the game on collision
    } else {
        P1OUT |= LED_GREEN; // Turn on green LED for successful dodge
    }
}

void __attribute__((interrupt(WDT_VECTOR))) WDT_ISR() {
    timerHandler(); // Handle timer interrupts for game logic
}

void timerHandler() {
    static unsigned char timerCount = 0;

    if (++timerCount == 50) { // Every ~1 second (assuming 20ms per WDT tick)
        fallingObject = (fallingObject + 1) % 3; // Move falling object
        timerCount = 0;
    }
}
