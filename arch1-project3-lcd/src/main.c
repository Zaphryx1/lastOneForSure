
#include <msp430.h>
#include "lcdutils.h"
#include "lcddraw.h"

// Game state
#define STATE_START 0
#define STATE_PLAY 1
#define STATE_GAME_OVER 2

volatile unsigned char gameState = STATE_START;

void main() {
    configureClocks();
    lcd_init();
    clearScreen(COLOR_BLACK);

    P1IE |= BIT3;  // Enable interrupt for button on P1.3
    P1IFG &= ~BIT3; // Clear interrupt flag
    enableWDTInterrupts();
    or_sr(0x18); // CPU off, GIE on

    while (1) {
        clearScreen(COLOR_BLACK);
        if (gameState == STATE_START) {
            drawString5x7(20, 50, "Press Start", COLOR_GREEN, COLOR_BLACK);
        } else if (gameState == STATE_PLAY) {
            drawString5x7(20, 50, "Game Running", COLOR_WHITE, COLOR_BLACK);
        } else if (gameState == STATE_GAME_OVER) {
            drawString5x7(20, 50, "Game Over", COLOR_RED, COLOR_BLACK);
        }
        or_sr(0x10); // Low-power mode
    }
}

// Button interrupt handler
void __interrupt_vec(PORT1_VECTOR) Port_1() {
    if (P1IFG & BIT3) {
        P1IFG &= ~BIT3; // Clear flag
        if (gameState == STATE_START) {
            gameState = STATE_PLAY;
        } else if (gameState == STATE_PLAY) {
            gameState = STATE_GAME_OVER;
        } else {
            gameState = STATE_START;
        }
    }
}

// Watchdog timer interrupt handler
void __interrupt_vec(WDT_VECTOR) WDT() {
    // Add periodic game updates here if necessary
}
