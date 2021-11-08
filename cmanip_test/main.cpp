#include <windows.h>

#include <iostream>
using namespace std;

#include "cmanip.h"

int main() {
  setConsoleFont(L"Consolas", FW_BOLD, 10, 20);
  cout << "\u2513" << endl;
  return 0;
}
