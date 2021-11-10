#include "main_utils.h"

#include <iostream>

#include "Windows.h"
#include "cmanip.h"
#include "global.h"

void resetConsoleScreen() {
  setConsoleColor(CONSOLE_BACKGROUND_COLOR, CONSOLE_TEXT_COLOR);
  clearConsole();
}

int getStartPositionOfACenteredText(const int &textSize) {
  return (WINDOW_WIDTH - textSize) / 2;
}

int printCenteredText(const std::string &text, const int &coordY) {
  int coordX = getStartPositionOfACenteredText(text.size());
  setConsoleCursorPosition(coordX, coordY);
  std::cout << text;
  return coordX;
}

void printColoredTextWrapper(std::function<void()> func,
                             const int &backgroundColor, const int &textColor) {
  setConsoleTextColor(backgroundColor, textColor);
  func();
  setConsoleTextColor(CONSOLE_BACKGROUND_COLOR, CONSOLE_TEXT_COLOR);
}

int mod(const int &a, const int &b) { return (a % b + b) % b; }

void setupWindow() {
  setConsoleFont(L"Consolas", FW_BOLD, 12, 24);
  setConsoleColor(CONSOLE_BACKGROUND_COLOR, CONSOLE_TEXT_COLOR);
  setWindowName("Minesweeper v1.0");
  setWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
  fixWindowSize();
  disableConsoleQuickEditMode();
  showConsoleCursor(false);
}
