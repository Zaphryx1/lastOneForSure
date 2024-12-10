
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
    } else if (!gameStarted) { // Any button starts the game
        gameStarted = 1;
        P2IFG &= ~(BIT0 | BIT1 | BIT2 | BIT3); // Clear all flags
    }
}

void __attribute__((interrupt(PORT2_VECTOR))) Port_2() {
    if (P2IFG & BIT0) { // Button S1 pressed
        moveCharacter(1); // Move left
        P2IFG &= ~BIT0; // Clear flag
    } else if (P2IFG & BIT1) { // Button S2 pressed
        moveCharacter(2); // Move right
        P2IFG &= ~BIT1; // Clear flag
    } else if (P2IFG & BIT2) { // Button S3 pressed (Pause)
        static unsigned char paused = 0;
        paused = !paused; // Toggle pause state
        if (paused) {
            P1OUT &= ~(LED_RED | LED_GREEN); // Turn off LEDs
            buzzer_set_period(0);            // Silence buzzer
            __bis_SR_register(LPM0_bits);    // Enter low power mode
        } else {
            __bic_SR_register_on_exit(LPM0_bits); // Exit low power mode
        }
        P2IFG &= ~BIT2; // Clear flag
    }
}
