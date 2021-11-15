#include "Leaderboard.h"

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>

#include "Leaderboard.h"
#include "cmanip.h"
#include "game_model.h"
#include "global.h"
#include "main_utils.h"
#include "scene_manager.h"

const std::string LEADERBOARD_FILE_NAME = "leaderboard.txt";
const int MAX_TIME = 1e6;

// 3 che do
// 0 0 0 0 0 0 0 0 0 0
// 0 0 0 0 0 0 0 0 0 0
// 0 0 0 0 0 0 0 0 0 0
long long leaderboard[NUM_LEVELS][NUM_PLAYERS_PER_LEVEL + 1];

void initLeaderboardFile() {
  std::ofstream dataFile(LEADERBOARD_FILE_NAME);

  for (int level = 0; level < NUM_LEVELS; level++) {
    for (int player = 0; player < NUM_PLAYERS_PER_LEVEL; player++) {
      dataFile << MAX_TIME << " ";
      leaderboard[level][player] = MAX_TIME;
    }
    dataFile << '\n';
  }

  dataFile.close();
}

void loadLeaderboardData() {
  std::ifstream dataFile(LEADERBOARD_FILE_NAME);

  if (!dataFile) {
    initLeaderboardFile();
    return;
  }

  for (int i = 0; i < NUM_LEVELS; i++)
    for (int j = 0; j < NUM_PLAYERS_PER_LEVEL; j++)
      dataFile >> leaderboard[i][j];

  dataFile.close();
}

void updateLeaderboardData() {
  std::ofstream dataFile(LEADERBOARD_FILE_NAME);

  for (int level = 0; level < NUM_LEVELS; level++) {
    for (int player = 0; player < NUM_PLAYERS_PER_LEVEL; player++)
      dataFile << leaderboard[level][player] << " ";
    dataFile << '\n';
  }

  dataFile.close();
}

void addToLeaderboard(const int& level, const long long& elapsedTime) {
  leaderboard[level][NUM_PLAYERS_PER_LEVEL] = elapsedTime;
  std::sort(leaderboard[level], leaderboard[level] + NUM_PLAYERS_PER_LEVEL + 1);

  if (elapsedTime != leaderboard[level][NUM_PLAYERS_PER_LEVEL])
    updateLeaderboardData();
}

void displayLeaderboard() {
  resetConsoleScreen();

  for (int level = 0; level < NUM_LEVELS; level++) {
    for (int player = 0; player < NUM_PLAYERS_PER_LEVEL; player++)
      if (leaderboard[level][player] == MAX_TIME)
        std::cout << "-/- ";
      else
        std::cout << std::fixed << std::setprecision(3)
                  << leaderboard[level][player] / 1000.0 << " ";
    std::cout << '\n';
  }
}

void resetLeaderboard() {
  printCenteredText(
      "Do you want to reset leaderboard (all data will be lost!) ?", 9);
  printCenteredText("[Y] Yes / [N] No", 10);

  while (true) {
    int action = getUserAction();
    if (action == YES) {
      initLeaderboardFile();
      displayLeaderboard();
      return;
    }

    else if (action == NO || action == ESCAPE) {
      return;
    }
  }
}

int Leaderboard() {
  displayLeaderboard();
  if (getUserAction() == RESET) {
    resetLeaderboard();
    getUserAction();
  }
  return WELCOME;
}
