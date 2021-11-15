#include "main_utils.h"

#include <iostream>
#include <sstream>

#include "cmanip.h"
#include "global.h"
#include "windows.h"

void resetConsoleScreen() {
  setConsoleColor(CONSOLE_BACKGROUND_COLOR, CONSOLE_TEXT_COLOR);
  clearConsole();
}

int getStartPositionOfACenteredText(const int &textSize) {
  return (getWindowWidth() - textSize) / 2;
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
  setConsoleFont(L"Consolas", FW_BOLD, FONT_WIDTH, FONT_HEIGHT);
  setConsoleColor(CONSOLE_BACKGROUND_COLOR, CONSOLE_TEXT_COLOR);
  setWindowName("Minesweeper v1.0");
  setWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
  fixWindowSize();
  disableConsoleQuickEditMode();
  showConsoleCursor(false);
}

std::string intToString(const int &number) {
  std::stringstream stream;
  stream << number;
  return stream.str();
}

long long getTimeDiff(
    std::chrono::high_resolution_clock::time_point timePoint1,
    std::chrono::high_resolution_clock::time_point timePoint2) {
  return std::chrono::duration_cast<std::chrono::milliseconds>(timePoint2 -
                                                               timePoint1)
      .count();
}
