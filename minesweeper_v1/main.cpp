#include <iostream>

#include "cmanip.h"

using namespace std;

const int WINDOW_WIDTH = 50, WINDOW_HEIGHT = 20;
const int MAP_WIDTH = 20, MAP_HEIGHT = 20;

const int CONSOLE_BACKGROUND_COLOR = BRIGHT_WHITE;
const int CONSOLE_TEXT_COLOR = BLACK;

void setup() {
  setConsoleColor(CONSOLE_BACKGROUND_COLOR, CONSOLE_TEXT_COLOR);
  setWindowName("Minesweeper v1.0");
  setWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
  fixWindowSize();
  showConsoleCursor(0);
}

void setupInput() {
  setConsoleColor(CONSOLE_BACKGROUND_COLOR, CONSOLE_TEXT_COLOR);
  clearConsole();
}

void input() { setupInput(); }

void startGame() {
  for (int row = 0; row < MAP_WIDTH; ++row) {
    for (int column = 0; column < MAP_HEIGHT; ++column) {
      cout << "@";
    }
    cout << '\n';
  }
}

void game() {
  input();
  startGame();
}

int main() {
  setup();
  game();
  return 0;
}
