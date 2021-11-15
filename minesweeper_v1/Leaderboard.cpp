#include "Leaderboard.h"

#include <algorithm>
#include <iomanip>
#include <iostream>

#include "Leaderboard.h"
#include "game_model.h"
#include "global.h"
#include "main_utils.h"
#include "scene_manager.h"

long long leaderboard[NUM_LEVELS][NUM_PLAYERS_PER_LEVEL + 1];
bool isFirstCall = true;

void resetLeaderboard(long long leaderboard[][NUM_PLAYERS_PER_LEVEL + 1]) {
  for (int level = 0; level < NUM_LEVELS; level++)
    for (int player = 0; player < NUM_PLAYERS_PER_LEVEL; player++)
      leaderboard[level][player] = 0;
}

void getOldLeaderboardData() {
  resetLeaderboard(leaderboard);
  transferDataToLeaderboard(leaderboard);
}

bool addToLeaderboard(const int& level, const long long& elapsedTime,
                      long long savedLeaderboard[][NUM_PLAYERS_PER_LEVEL + 1]) {
  if (isFirstCall) {
    getOldLeaderboardData();
    isFirstCall = false;
  }

  leaderboard[level][NUM_PLAYERS_PER_LEVEL] = elapsedTime;
  std::sort(leaderboard[level], leaderboard[level] + NUM_PLAYERS_PER_LEVEL + 1);

  if (elapsedTime == leaderboard[level][NUM_PLAYERS_PER_LEVEL])
    return false;
  else {
    for (int mode = 0; mode < NUM_LEVELS; mode++)
      for (int player = 0; player < NUM_PLAYERS_PER_LEVEL; player++)
        savedLeaderboard[mode][player] = leaderboard[mode][player];
    return true;
  }
}

void displayLeaderboard() {
  for (int level = 0; level < NUM_LEVELS; level++) {
    for (int player = 0; player < NUM_PLAYERS_PER_LEVEL; player++)
      if (leaderboard[level][player] == 0)
        std::cout << "---- ";
      else
        std::cout << std::fixed << std::setprecision(3)
                  << leaderboard[level][player] / 1000.0 << " ";
    std::cout << '\n';
  }
}

int Leaderboard() {
  resetConsoleScreen();
  displayLeaderboard();
  getUserAction();
  return WELCOME;
}
