
#ifndef SIMPLIFIED_GAME_AND_WATCH_H
#define SIMPLIFIED_GAME_AND_WATCH_H

#define LED_RED BIT0
#define LED_GREEN BIT6

void moveCharacter(unsigned char direction);
void checkCollision();
void drawStartScreen();
void drawGameScreen();

#endif // SIMPLIFIED_GAME_AND_WATCH_H
