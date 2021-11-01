#include <iostream>
#include "cmanip.h"
using namespace std;

int main() {
  setConsoleColor(LIGHT_GREEN, LIGHT_BLUE);
  setWindowSize(30, 20);
  setConsoleCursorPosition(10, 20);
  setConsoleTextColor(YELLOW, RED);
  cout << "Test";
  return 0;
}
