#ifndef CMANIP_H_INCLUDED
#define CMANIP_H_INCLUDED

#include <string>

enum colors {
  BLACK,
  BLUE,
  GREEN,
  AQUA,
  RED,
  PURPLE,
  YELLOW,
  WHITE,
  GRAY,
  LIGHT_BLUE,
  LIGHT_GREEN,
  LIGHT_AQUA,
  LIGHT_RED,
  LIGHT_PURPLE,
  LIGHT_YELLOW,
  BRIGHT_WHITE
};

void setWindowName(const std::string& name);
void setWindowSize(const short& width, const short& height);
void fixWindowSize();
void setConsoleColor(const int& backgroundColor, const int& textColor);
void showConsoleCursor(const bool& flag);
void setConsoleCursorPosition(const short& x, const short& y);
void setConsoleTextColor(const int& backgroundColor, const int& textColor);
void clearConsole();

#endif  // CMANIP_H_INCLUDED
