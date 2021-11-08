#include "cmanip.h"

#include <conio.h>
#include <windows.h>

#include <map>
#include <string>

#include "cmanip_utils.h"

void setWindowName(const std::string& name) {
  SetConsoleTitle(std::wstring(name.begin(), name.end()).c_str());
}

void setWindowSize(const short& width, const short& height) {
  COORD pos{width, height};
  SMALL_RECT rect;
  HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);

  rect = {0, 0, 1, 1};
  SetConsoleWindowInfo(handle, TRUE, &rect);
  SetConsoleScreenBufferSize(handle, pos);
  SetConsoleActiveScreenBuffer(handle);

  rect = {0, 0, (short)(width - 1), (short)(height - 1)};
  SetConsoleWindowInfo(handle, TRUE, &rect);
  SetConsoleActiveScreenBuffer(handle);
  SetConsoleScreenBufferSize(handle, pos);
}

void fixWindowSize() {
  HWND consoleWindow = GetConsoleWindow();
  SetWindowLong(
      consoleWindow, GWL_STYLE,
      GetWindowLong(consoleWindow, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);
}

void setConsoleFont(const wchar_t fontName[], const short& fontWeight,
                    const short& width, const short& height) {
  CONSOLE_FONT_INFOEX cfi;
  cfi.cbSize = sizeof cfi;
  cfi.nFont = 0;
  cfi.dwFontSize = {width, height};
  cfi.FontFamily = FF_DONTCARE;
  cfi.FontWeight = fontWeight;
  wcscpy(cfi.FaceName, fontName);
  SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
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

void clearConsole() { system("cls"); }

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
  int character;
  do {
    character = getch();
    if (character == 0 || character == 224) {
      // checking if the key is an extended one
      // to ignore the additional character when dealing with arrow keys
      character = getch();
    }
  } while (inputMap.find(character) == inputMap.end());
  return inputMap[character];
}
