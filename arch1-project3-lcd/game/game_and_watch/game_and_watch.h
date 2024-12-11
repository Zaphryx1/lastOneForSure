
#ifndef GAME_AND_WATCH_H
#define GAME_AND_WATCH_H

#define LED_RED BIT0
#define LED_GREEN BIT6

// Function prototypes
void moveCharacter(unsigned char direction);
void checkCollision();
void drawStartScreen();
void drawGameScreen();

#endif // GAME_AND_WATCH_H
