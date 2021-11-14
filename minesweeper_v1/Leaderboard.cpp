#include "Leaderboard.h"

#include <algorithm>
#include <iostream>

#include "Leaderboard.h"
#include "game_model.h"
#include "global.h"
#include "main_utils.h"
#include "scene_manager.h"

long long leaderboard[NUM_LEVELS][NUM_RECORDS_PER_LEVEL + 1];
bool isFirstCall = true;

void resetLeaderboard(long long leaderboard[][NUM_RECORDS_PER_LEVEL + 1]) {
  for (int mode = 0; mode < NUM_LEVELS; mode++)
    for (int player = 0; player < NUM_RECORDS_PER_LEVEL; player++)
      leaderboard[mode][player] = 0;
}

void getOldLeaderboardData() {
  resetLeaderboard(leaderboard);
  transferDataToLeaderboard(leaderboard);
}

bool addToLeaderboard(int MODE, long long point,
                      long long savedLeaderboard[][NUM_RECORDS_PER_LEVEL]) {
  if (isFirstCall) {
    getOldLeaderboardData();
    isFirstCall = false;
  }

  leaderboard[MODE][NUM_RECORDS_PER_LEVEL] = point;
  std::sort(leaderboard[MODE], leaderboard[MODE] + NUM_RECORDS_PER_LEVEL + 1,
            std::greater<long long>());

  if (point == leaderboard[MODE][NUM_RECORDS_PER_LEVEL])
    return false;
  else {
    for (int mode = 0; mode < NUM_LEVELS; mode++)
      for (int player = 0; player < NUM_RECORDS_PER_LEVEL; player++)
        savedLeaderboard[mode][player] = leaderboard[mode][player];
    return true;
  }
}

void displayLeaderboard() {
  for (int mode = 0; mode < NUM_LEVELS; mode++) {
    for (int player = 0; player < NUM_RECORDS_PER_LEVEL; player++)
      if (leaderboard[mode][player] == 0)
        std::cout << "---- ";
      else
        std::cout << leaderboard[mode][player] << " ";
    std::cout << '\n';
  }
}

int Leaderboard() {
  resetConsoleScreen();
  displayLeaderboard();
  getUserAction();
  return WELCOME;
}
