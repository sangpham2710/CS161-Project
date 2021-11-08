#include <iostream>

#include "cmanip.h"

int main() {
  setConsoleColor(LIGHT_GREEN, LIGHT_BLUE);
  setWindowSize(80, 30);
  setConsoleCursorPosition(1, 1);
  setConsoleTextColor(YELLOW, RED);
  std::cout << "Hello World";
  return 0;
}
