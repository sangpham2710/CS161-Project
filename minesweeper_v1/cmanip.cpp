#include "cmanip.h"

#include <conio.h>
#include <windows.h>

#include <map>
#include <string>

#include "cmanip_utils.h"

void setWindowName(const std::string& name) {
  std::string tmp = std::string("title ") + name;
  system(tmp.c_str());
}

void setWindowSize(const short& width, const short& height) {
  COORD pos{width, height};
  SMALL_RECT rect{0, 0, width - 1, height - 1};
  HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
  SetConsoleScreenBufferSize(handle, pos);
  SetConsoleWindowInfo(handle, TRUE, &rect);
}

void fixWindowSize() {
  HWND consoleWindow = GetConsoleWindow();
  SetWindowLong(
    consoleWindow, GWL_STYLE,
    GetWindowLong(consoleWindow, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);
}

void setConsoleColor(const int& backgroundColor, const int& textColor) {
  std::string tmp = std::string("color ") +
                    intToHexString((backgroundColor << 4) | textColor);
  system(tmp.c_str());
}

void showConsoleCursor(const bool& flag) {
  HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_CURSOR_INFO tmp;
  GetConsoleCursorInfo(handle, &tmp);
  tmp.bVisible = flag;
  SetConsoleCursorInfo(handle, &tmp);
}

void setConsoleCursorPosition(const short& x, const short& y) {
  COORD pos{x, y};
  HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
  SetConsoleCursorPosition(handle, pos);
}

void setConsoleTextColor(const int& backgroundColor, const int& textColor) {
  HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
  SetConsoleTextAttribute(handle, (backgroundColor << 4) | textColor);
}

void clearConsole() {
  system("cls");
}

std::map<int, int> inputMap = {
  {'w', UP},     {'W', UP},     {UP_ARROW_KEY, UP},
  {'s', DOWN},   {'S', DOWN},   {DOWN_ARROW_KEY, DOWN},
  {'a', LEFT},   {'A', LEFT},   {LEFT_ARROW_KEY, LEFT},
  {'d', RIGHT},  {'D', RIGHT},  {RIGHT_ARROW_KEY, RIGHT},
  {'j', MOUSE1}, {'J', MOUSE1}, {RETURN_KEY, MOUSE1},
  {'k', MOUSE2}, {'K', MOUSE2}, {'l', MOUSE3},
  {'L', MOUSE3}, {'y', YES},    {'Y', YES},
  {'n', NO},     {'N', NO},     {ESCAPE_KEY, ESCAPE},
};

int getUserAction() {
  int character = getch();
  if (character == 0 || character == 224) {
    // checking if the key is an extended one
    // to ignore the additional character when dealing with arrow keys
    character = getch();
  }
  return inputMap[character];
}
