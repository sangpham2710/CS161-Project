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

enum keyCodes {
  UP_ARROW_KEY = 72,
  DOWN_ARROW_KEY = 80,
  LEFT_ARROW_KEY = 75,
  RIGHT_ARROW_KEY = 77,
  RETURN_KEY = 13,
  ESCAPE_KEY = 27
};

enum actionTypes {
  NO_ACTION,
  UP,
  DOWN,
  LEFT,
  RIGHT,
  ESCAPE,
  MOUSE1,
  MOUSE2,
  MOUSE3,
  YES,
  NO
};

void setWindowName(const std::string& name);
void setWindowSize(const short& width, const short& height);
void fixWindowSize();
void setConsoleFont(const wchar_t fontName[], const short& fontWeight,
                    const short& width, const short& height);
void setConsoleColor(const int& backgroundColor, const int& textColor);
void disableConsoleQuickEditMode();
void showConsoleCursor(const bool& flag);
void setConsoleCursorPosition(const short& x, const short& y);
void setConsoleTextColor(const int& backgroundColor, const int& textColor);
void clearConsole();
int getUserAction();

#endif  // CMANIP_H_INCLUDED
