
#include <msp430.h>
#include "mr_game_watch.h"

void initButtons() {
    P2DIR &= ~(BIT0 | BIT1 | BIT2 | BIT3); // Set buttons as input
    P2REN |= (BIT0 | BIT1 | BIT2 | BIT3); // Enable pull-up/down resistors
    P2OUT |= (BIT0 | BIT1 | BIT2 | BIT3); // Pull-up selected
    P2IE |= (BIT0 | BIT1 | BIT2 | BIT3);  // Enable interrupts
    P2IES |= (BIT0 | BIT1 | BIT2 | BIT3); // High-to-low transition
    P2IFG &= ~(BIT0 | BIT1 | BIT2 | BIT3); // Clear interrupt flags
}

// Port 2 ISR for button presses
void __attribute__((interrupt(PORT2_VECTOR))) Port_2() {
    if (P2IFG & BIT0) { // Button S1 pressed
        moveCharacter(1); // Move left
        P2IFG &= ~BIT0; // Clear flag
    } else if (P2IFG & BIT1) { // Button S2 pressed
        moveCharacter(2); // Move right
        P2IFG &= ~BIT1; // Clear flag
    } else if (P2IFG & BIT2) { // Button S3 pressed (Pause/Resume)
        togglePause();
        P2IFG &= ~BIT2; // Clear flag
    } else if (P2IFG & BIT3) { // Button S4 pressed (Start Game)
        gameStarted = 1;
        P2IFG &= ~BIT3; // Clear flag
    }
}
