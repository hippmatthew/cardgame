#include "include/graphics.h"
#include "include/game.h"

#include <stdio.h>
#include <termios.h>
#include <unistd.h>

void processInput(int);
const char * readIP();

int main() {
  initShell();

  char sel = 0;
  while (sel != -1) {
    clearScreen();
    drawMenu(sel);

    char input = getchar();
    switch (input) {
      case 'w':
        sel = sel == 0 ? 0 : sel - 1;
        break;
      case 's':
        sel = sel == 2 ? 2 : sel + 1;
        break;
      case 'e':
        processInput(sel);
        sel = -1;
      default:
        continue;
    }
  }

  deinitShell();

  return 0;
}

void processInput(int sel) {
  switch (sel) {
    case 0:
      playHost();
      break;
    case 1:
      playClient(readIP());
      break;
    default:
      clearScreen();
  }
}

const char * readIP() {
  (void)printf("\033[4;8H\033[?25h-- ip: ");
  (void)fflush(stdout);

  static char buf[16] = {0};
  int size = 0;

  char input = getchar();
  while (input != '\n' && size < 15) {
    buf[size] = input;

    (void)printf("\033[4;%dH%c", 15 + size++, input);
    (void)fflush(stdout);

    input = getchar();
  }
  (void)printf("\033[?25l");
  (void)fflush(stdout);

  buf[size] = '\0';

  return buf;
}