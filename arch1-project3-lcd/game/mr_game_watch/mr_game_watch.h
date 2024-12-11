
#ifndef MR_GAME_WATCH_H
#define MR_GAME_WATCH_H

#define LED_RED BIT0
#define LED_GREEN BIT6

// Function prototypes
void moveCharacter(unsigned char direction);
void checkCollision();
void showStartScreen();
void drawGameScreen();
void togglePause();
void initButtons();

#endif // MR_GAME_WATCH_H
