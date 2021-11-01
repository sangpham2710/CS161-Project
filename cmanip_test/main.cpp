#include <iostream>

#include "cmanip.h"

int main() {
  setConsoleColor(LIGHT_GREEN, LIGHT_BLUE);
  setWindowSize(30, 20);
  setConsoleCursorPosition(10, 20);
  setConsoleTextColor(YELLOW, RED);
  std::cout << "Hello World";
  return 0;
}
