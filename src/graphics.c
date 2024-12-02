#include "include/graphics.h"

#include <stdio.h>
#include <termios.h>
#include <unistd.h>

void drawCard(card_t card, int x, int y) {
  (void)printf("\033[%d;%dH--------------", y + 1, x + 1);

  for (int i = 1; i <= 6; ++i) {
    (void)printf("\033[%d;%dH|", y + i + 1, x + 1);

    switch (i) {
      case 1:
        (void)printf(
          "\033[%d;%dH%s%c", y + i + 1, x + 3, card_char(card.value), card.value != JOKER ? suit_char(card.suit) : ' '
        );
        break;
      case 3:
        (void)printf("\033[%d;%dH%s", y + i + 1, x + 5, card_str(card.value));
        break;
      case 4:
        (void)printf("\033[%d;%dH%s", y + i + 1, x + 5, card.value != JOKER ? suit_str(card.suit) : "        ");
        break;
      case 6:
        (void)printf(
          "\033[%d;%dH%s%c", y + i + 1, x + 11, card_char(card.value), card.value != JOKER ? suit_char(card.suit) : ' '
        );
        break;
      default:
        break;
    }

    (void)printf("\033[%d;%dH|", y + i + 1, x + 14);
  }

  (void)printf("\033[%d;%dH--------------", y + 8, x + 1);
  (void)fflush(stdout);
}

void drawOppCard(int x, int y) {
  (void)printf("\033[%d;%dH-------",y + 1, x + 1);
  for (int i = 1; i <=3; ++i)
    (void)printf("\033[%d;%dH|*****|", y + i + 1, x + 1);
  (void)printf("\033[%d;%dH-------", y + 5, x + 1);
  (void)fflush(stdout);
}

void drawOppHand(int cardCount) {
  int x = 15;
  int y = 2;
  for (int i = 0; i < cardCount; ++i) {
    drawOppCard(x, y);
    x += 8;
  }
}

void initShell() {
  struct termios terminal;

  (void)tcgetattr(STDIN_FILENO, &terminal);
  terminal.c_lflag &= ~(ICANON | ECHO);
  (void)tcsetattr(STDIN_FILENO, TCSANOW, &terminal);

  (void)printf("\033[?25l");
  (void)fflush(stdout);
}

void deinitShell() {
  struct termios terminal;

  (void)tcgetattr(STDIN_FILENO, &terminal);
  terminal.c_lflag |= (ICANON | ECHO);
  (void)tcsetattr(STDIN_FILENO, TCSANOW, &terminal);

  (void)printf("\033[?25h\033[41;1H\033[2K");
  (void)fflush(stdout);
}

void clearScreen() {
  (void)printf("\033[2J\033[H");
  (void)fflush(stdout);
}

void drawMenu(char sel) {
  (void)printf("=== DROP 4 ===\n\n");

  if (sel == 0) (void)printf("> ");
  (void)printf("Host\n");

  if (sel == 1) (void)printf("> ");
  (void)printf("Join\n");

  if (sel == 2) (void)printf("> ");
  (void)printf("Quit\n");
}

void drawBoard(struct game_state * state) {
  (void)printf(
    "\033[1;1HRound %d\t\tDeck: %d\t\tS-%d  H-%d  D-%d  C-%d",
    state->round,
    state->deck.count,
    state->suitValues[0],
    state->suitValues[1],
    state->suitValues[2],
    state->suitValues[3]
  );
  (void)fflush(stdout);

  drawOppHand(state->oppCardCount);
}

void drawHand(int sel, hand_t hand) {
  for (int i = 0; i < 10; ++i)
    (void)printf("\033[%d;1H\033[2K", 30 + i + 1);
  (void)fflush(stdout);

  int x = 0;
  for (int i = 0; i < hand.count; ++i) {
    drawCard(hand.cards[i], x, sel == i ? 30 : 32);
    x += 15;
  }
}

void drawChosen(card_t playerCard, card_t oppCard) {
  drawCard(oppCard, 23, 10);
  drawCard(playerCard, 23, 19);
}