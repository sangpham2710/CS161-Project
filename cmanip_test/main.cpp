#include <windows.h>

#include <iostream>
using namespace std;

#include "cmanip.h"

int main() {
  setConsoleFont(L"Consolas", FW_BOLD, 10, 20);
  setWindowSize(40, 30);
  cout << getWindowWidth() << ' ' << getWindowHeight() << '\n';
  return 0;
}
