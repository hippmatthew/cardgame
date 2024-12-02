#ifndef gamble_graphics_h
#define gamble_graphics_h

#include "types.h"

void initShell();
void deinitShell();
void clearScreen();
void drawMenu(char);
void drawBoard(struct game_state *);
void drawHand(int, hand_t);
void drawChosen(card_t, card_t);
void drawOppHand(int);

#endif