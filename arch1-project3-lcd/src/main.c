
#include <msp430.h>
#include "libTimer.h"
#include "lcdutils.h"   // For lcd_init and COLOR constants
#include "lcddraw.h"    // For graphics drawing functions
#include "buzzer.h"     // For sound management

// Game state constants
#define STATE_START 0
#define STATE_PLAY 1
#define STATE_PAUSE 2
#define STATE_GAME_OVER 3

volatile unsigned char gameState = STATE_START;
volatile unsigned char playerPosition = 1;
volatile unsigned char fallingToolPosition = 0;
volatile unsigned char fallingToolLane = 1;
volatile unsigned int score = 0;

// Button interrupt handler
void __interrupt_vec(PORT1_VECTOR) Port_1() {
    if (P1IFG & BIT3) { // Check if button on P1.3 was pressed
        P1IFG &= ~BIT3; // Clear interrupt flag
        if (gameState == STATE_START || gameState == STATE_GAME_OVER) {
            gameState = STATE_PLAY;
            clearScreen(COLOR_BLACK);
        } else if (gameState == STATE_PLAY) {
            gameState = STATE_PAUSE;
        } else if (gameState == STATE_PAUSE) {
            gameState = STATE_PLAY;
        }
    }
}

// Timer interrupt for updating game
void __interrupt_vec(WDT_VECTOR) WDT() {
    static int timerCount = 0;
    if (gameState == STATE_PLAY) {
        timerCount++;
        if (timerCount == 25) { // Adjust frequency
            fallingToolPosition++;
            if (fallingToolPosition >= 10) { // Reset tool
                fallingToolPosition = 0;
                fallingToolLane = (fallingToolLane + 1) % 3;
            }
            if (fallingToolPosition == 9 && fallingToolLane == playerPosition) {
                gameState = STATE_GAME_OVER;
            }
            timerCount = 0;
        }
    }
}

void drawGame() {
    if (gameState == STATE_PLAY) {
        drawString5x7(10, 10, "Avoid the tools!", COLOR_WHITE, COLOR_BLACK);
        drawChar(20, playerPosition * 30, 'P', COLOR_GREEN, COLOR_BLACK); // Player character
        drawChar(fallingToolPosition * 10, fallingToolLane * 30, 'T', COLOR_RED, COLOR_BLACK); // Tool
    } else if (gameState == STATE_GAME_OVER) {
        drawString5x7(30, 50, "Game Over", COLOR_RED, COLOR_BLACK);
    } else if (gameState == STATE_START) {
        drawString5x7(30, 50, "Press Start!", COLOR_GREEN, COLOR_BLACK);
    }
}

void main() {
    configureClocks();
    lcd_init();
    clearScreen(COLOR_BLACK);
    P1IE |= BIT3;       // Enable interrupts for button
    enableWDTInterrupts();
    or_sr(0x18);        // CPU off, GIE on

    while (1) {
        drawGame();
        or_sr(0x10);    // Low-power mode
    }
}
