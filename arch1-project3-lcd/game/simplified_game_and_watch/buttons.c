
#include <msp430.h>
#include "simplified_game_and_watch.h"

extern unsigned char position;
extern unsigned char paused;

void initButtons() {
    P2DIR &= ~(BIT0 | BIT1 | BIT2); // Set buttons as input
    P2REN |= (BIT0 | BIT1 | BIT2);  // Enable pull-up/down resistors
    P2OUT |= (BIT0 | BIT1 | BIT2);  // Pull-up selected
    P2IE |= (BIT0 | BIT1 | BIT2);   // Enable interrupts
    P2IES |= (BIT0 | BIT1 | BIT2);  // High-to-low transition
    P2IFG &= ~(BIT0 | BIT1 | BIT2); // Clear interrupt flags
}

// Port 2 ISR for button presses
void __attribute__((interrupt(PORT2_VECTOR))) Port_2() {
    if (P2IFG & BIT0) { // Button S1 pressed
        moveCharacter(1); // Move left
        P2IFG &= ~BIT0;  // Clear flag
    } else if (P2IFG & BIT1) { // Button S2 pressed
        moveCharacter(2); // Move right
        P2IFG &= ~BIT1;   // Clear flag
    } else if (P2IFG & BIT2) { // Button S3 pressed
        paused = !paused; // Toggle pause
        P2IFG &= ~BIT2;   // Clear flag
    }
}
