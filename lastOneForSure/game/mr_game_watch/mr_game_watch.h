
#ifndef MR_GAME_WATCH_H
#define MR_GAME_WATCH_H

#define LED_RED BIT0
#define LED_GREEN BIT6

// Function prototypes
void drawGameScreen();
void playTune(const char *tune);
void moveCharacter(unsigned char direction);
void checkCollision();
void showStartScreen();

#endif // MR_GAME_WATCH_H
