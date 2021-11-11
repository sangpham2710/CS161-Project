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
  SMALL_RECT rect;
  HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

  rect = {0, 0, 1, 1};
  SetConsoleWindowInfo(hOut, TRUE, &rect);
  SetConsoleScreenBufferSize(hOut, pos);
  SetConsoleActiveScreenBuffer(hOut);

  rect = {0, 0, (short)(width - 1), (short)(height - 1)};
  SetConsoleWindowInfo(hOut, TRUE, &rect);
  SetConsoleActiveScreenBuffer(hOut);
  SetConsoleScreenBufferSize(hOut, pos);
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

void disableConsoleQuickEditMode() {
  DWORD prevMode;
  HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
  GetConsoleMode(hIn, &prevMode);
  SetConsoleMode(hIn,
                 ENABLE_EXTENDED_FLAGS | (prevMode & ~ENABLE_QUICK_EDIT_MODE));
}

void showConsoleCursor(const bool& flag) {
  HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_CURSOR_INFO tmp;
  GetConsoleCursorInfo(hOut, &tmp);
  tmp.bVisible = flag;
  SetConsoleCursorInfo(hOut, &tmp);
}

void setConsoleCursorPosition(const short& x, const short& y) {
  COORD pos{x, y};
  HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
  SetConsoleCursorPosition(hOut, pos);
}

void setConsoleTextColor(const int& backgroundColor, const int& textColor) {
  HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
  SetConsoleTextAttribute(hOut, (backgroundColor << 4) | textColor);
}

void clearConsole() { system("cls"); }

std::map<int, int> inputMap = {
    {'w', UP},        {'W', UP},        {UP_ARROW_KEY, UP},
    {'s', DOWN},      {'S', DOWN},      {DOWN_ARROW_KEY, DOWN},
    {'a', LEFT},      {'A', LEFT},      {LEFT_ARROW_KEY, LEFT},
    {'d', RIGHT},     {'D', RIGHT},     {RIGHT_ARROW_KEY, RIGHT},
    {'j', MOUSE1},    {'J', MOUSE1},    {RETURN_KEY, MOUSE1},
    {'k', MOUSE2},    {'K', MOUSE2},    {'l', MOUSE3},
    {'L', MOUSE3},    {'y', YES},       {'Y', YES},
    {'n', NO},        {'N', NO},        {ESCAPE_KEY, ESCAPE},
    {'o', SAVE_GAME}, {'O', SAVE_GAME},
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
