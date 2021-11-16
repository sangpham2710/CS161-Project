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
const long long MAX_TIME = (long long)1e18;

// 3 che do
// 0 0 0 0 0 0 0 0 0 0
// 0 0 0 0 0 0 0 0 0 0
// 0 0 0 0 0 0 0 0 0 0
long long leaderboard[NUM_LEVELS][NUM_PLAYERS_PER_LEVEL + 1];

void resetLeaderboardFile() {
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
    resetLeaderboardFile();
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

bool addToLeaderboard(const int& level, const long long& elapsedTime) {
  loadLeaderboardData();
  leaderboard[level][NUM_PLAYERS_PER_LEVEL] = elapsedTime;
  std::sort(leaderboard[level], leaderboard[level] + NUM_PLAYERS_PER_LEVEL + 1);

  if (elapsedTime != leaderboard[level][NUM_PLAYERS_PER_LEVEL]) {
    updateLeaderboardData();
    return true;
  } else
    return false;
}

void displayLeaderboard() {
  loadLeaderboardData();
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

void displayResetLeaderboard() {
  printCenteredText(
      "Do you want to reset leaderboard (all data will be lost!) ?", 9);
  printCenteredText("[Y] Yes / [N] No", 10);
}

int handleResetLeaderboard() {
  while (true) {
    int action = getUserAction();
    if (action == YES) {
      resetLeaderboardFile();
      break;
    } else if (action == NO || action == ESCAPE) {
      break;
    }
  }
  return LEADERBOARD;
}

int Leaderboard() {
  displayLeaderboard();
  while (true) {
    int action = getUserAction();
    if (action == MOUSE1 || action == ESCAPE) {
      return WELCOME;
    } else if (action == RESET) {
      displayResetLeaderboard();
      return handleResetLeaderboard();
    }
  }
}
