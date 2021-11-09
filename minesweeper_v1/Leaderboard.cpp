#include "Leaderboard.h"

#include <iostream>

#include "main_utils.h"
#include "scene_manager.h"

int Leaderboard() {
  resetConsoleScreen();
  std::cout << "Leaderboard\n";
  return WELCOME;
}