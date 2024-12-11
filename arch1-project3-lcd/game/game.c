
#include <msp430.h>
#include "lcdutils.h"
#include "lcddraw.h"

// Define player and object properties
#define PLAYER_COLOR COLOR_BLUE
#define OBJECT_COLOR COLOR_RED
#define BG_COLOR COLOR_BLACK
#define PLAYER_SIZE 10
#define OBJECT_SIZE 10

// Define button configurations
#define LEFT_BUTTON BIT0
#define RIGHT_BUTTON BIT1
#define PAUSE_BUTTON BIT2
#define SWITCHES (LEFT_BUTTON | RIGHT_BUTTON | PAUSE_BUTTON)

// Game state variables
unsigned char playerPosition = 1;  // Player position (0, 1, 2)
unsigned char objectPosition = 0; // Falling object position (0, 1, 2)
unsigned char objectRow = 0;      // Row of falling object
unsigned char paused = 1;         // Pause flag
unsigned int score = 0;           // Player score

// Function prototypes
void setup();
void drawGameScreen();
void updateObjectPosition();
void checkCollision();
void movePlayer(unsigned char direction);

void main() {
    WDTCTL = WDTPW | WDTHOLD; // Stop watchdog timer
    setup();

    while (1) {
        if (paused) {
            drawString5x7(20, 60, "Press S3 to Start", COLOR_WHITE, BG_COLOR);
        } else {
            drawGameScreen();
            checkCollision();
        }
        __bis_SR_register(LPM0_bits + GIE); // Enter low-power mode until interrupt
    }
}

void setup() {
    configureClocks();
    lcd_init();
    P1DIR &= ~SWITCHES;      // Set buttons as inputs
    P1REN |= SWITCHES;       // Enable pull-up/down resistors
    P1OUT |= SWITCHES;       // Pull-up selected
    P1IE |= SWITCHES;        // Enable interrupts
    P1IES |= SWITCHES;       // High-to-low transition
    P1IFG &= ~SWITCHES;      // Clear interrupt flags
    enableWDTInterrupts();   // Enable watchdog timer interrupts
}

void drawGameScreen() {
    // Clear only the necessary parts of the screen
    fillRectangle(0, objectRow - OBJECT_SIZE, 128, OBJECT_SIZE, BG_COLOR);

    // Draw player
    unsigned int playerX = playerPosition * 40 + 20;
    fillRectangle(playerX, 120, PLAYER_SIZE, PLAYER_SIZE, PLAYER_COLOR);

    // Draw falling object
    unsigned int objectX = objectPosition * 40 + 20;
    fillRectangle(objectX, objectRow, OBJECT_SIZE, OBJECT_SIZE, OBJECT_COLOR);

    // Display score
    char scoreStr[10];
    sprintf(scoreStr, "Score: %d", score);
    drawString5x7(5, 5, scoreStr, COLOR_WHITE, BG_COLOR);
}

void checkCollision() {
    if (objectRow >= 120 && objectPosition == playerPosition) {
        paused = 1;  // Pause the game on collision
        P1OUT |= BIT6; // Turn on red LED
    } else if (objectRow >= 120) {
        score++;        // Increment score on successful dodge
        objectRow = 0;  // Reset object position
        objectPosition = rand() % 3; // Randomize object position
    } else {
        objectRow += 10; // Move object down
    }
}

void movePlayer(unsigned char direction) {
    if (direction == 0 && playerPosition > 0) {
        playerPosition--; // Move left
    } else if (direction == 1 && playerPosition < 2) {
        playerPosition++; // Move right
    }
}

// Button interrupt service routine
void __attribute__((interrupt(PORT1_VECTOR))) Port_1() {
    if (P1IFG & LEFT_BUTTON) { // Left button pressed
        movePlayer(0);         // Move left
        P1IFG &= ~LEFT_BUTTON; // Clear interrupt flag
    } else if (P1IFG & RIGHT_BUTTON) { // Right button pressed
        movePlayer(1);         // Move right
        P1IFG &= ~RIGHT_BUTTON; // Clear interrupt flag
    } else if (P1IFG & PAUSE_BUTTON) { // Pause button pressed
        paused ^= 1;           // Toggle pause
        P1IFG &= ~PAUSE_BUTTON; // Clear interrupt flag
    }
}
