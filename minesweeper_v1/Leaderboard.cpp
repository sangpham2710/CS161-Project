#include "Leaderboard.h"

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

#include "Leaderboard.h"
#include "cmanip.h"
#include "game_model.h"
#include "global.h"
#include "main_utils.h"
#include "scene_manager.h"
#include "windows.h"

const std::string LEADERBOARD_FILE_NAME = "leaderboard.txt";
const long long MAX_TIME = (long long)1e18;

int PADDING_LB_X, PADDING_LB_Y;
const int leaderboardWidth =
  strlen("+------------+--------------+------------+"),
  leaderboardHeight = 14;
const int headingHeight = 3;

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

void displayResetLeaderboard() {
  printCenteredText(
    "+-------------------------------------------------------------+",
    getWindowHeight() / 2 - 1);
  printCenteredText(
    "| Do you want to reset leaderboard (all data will be lost!) ? |",
    getWindowHeight() / 2);
  printCenteredText(
    "|                                                             |",
    getWindowHeight() / 2 + 1);
  printCenteredText("[Y] Yes / [N] No", getWindowHeight() / 2 + 1);
  printCenteredText(
    "+-------------------------------------------------------------+",
    getWindowHeight() / 2 + 2);
}

int handleResetLeaderboard() {
  while (true) {
    int action = getUserAction();
    if (action == YES) {
      resetLeaderboardFile();
      break;
    } else if (action == NO || action == ESCAPE)
      break;
  }
  return LEADERBOARD;
}

int Leaderboard() {
  setupLeaderboardDisplay();
  displayLeaderboard();
  while (true) {
    int action = getUserAction();
    if (action == MOUSE1 || action == ESCAPE)
      return WELCOME;

    else if (action == RESET) {
      displayResetLeaderboard();
      return handleResetLeaderboard();
    }
  }
}

int distanceToCenterOfCell(const int &textSize, const int &cellWidth) {
  return (cellWidth - textSize) / 2;
}

void displayLeaderboard() {
  int cellWidth[NUM_LEVELS] = {12, 14, 12};

  loadLeaderboardData();
  resetConsoleScreen();
  printCenteredText("LEADERBOARD", PADDING_LB_Y);
  printCenteredText("Press [Enter] to return back, [R] to reset Leaderboard",
                    PADDING_LB_Y + 1);

  setConsoleCursorPosition(
    PADDING_LB_X,
    PADDING_LB_Y + headingHeight);
  std::cout << "+------------+--------------+------------+" << '\n';

  setConsoleCursorPosition(
    PADDING_LB_X,
    PADDING_LB_Y + headingHeight + 1);
  std::cout << "|  Beginner  | Intermediate |   Expert   |" << '\n';

  setConsoleCursorPosition(
    PADDING_LB_X,
    PADDING_LB_Y + headingHeight + 2);
  std::cout << "+------------+--------------+------------+" << '\n';

  for (int i = 0; i < NUM_PLAYERS_PER_LEVEL; i++) {
    setConsoleCursorPosition(
      PADDING_LB_X,
      PADDING_LB_Y + headingHeight + 3 + i);
    std::cout << "|            |              |            |" << '\n';
  }

  setConsoleCursorPosition(
    PADDING_LB_X,
    PADDING_LB_Y + headingHeight + 3 + NUM_PLAYERS_PER_LEVEL);
  std::cout << "+------------+--------------+------------+" << '\n';

  int totalWidth = 0;
  for (int mode = 0; mode < NUM_LEVELS; mode++) {
    for (int player = 0; player < NUM_PLAYERS_PER_LEVEL; player++) {
      if (leaderboard[mode][player] != MAX_TIME) {
        setConsoleCursorPosition(
          PADDING_LB_X + distanceToCenterOfCell(intToString(
              leaderboard[mode][player]).size() + 1, cellWidth[mode]) + totalWidth + 1,
          PADDING_LB_Y + headingHeight + 3 + player);

        std::cout << std::fixed << std::setprecision(3)
                  << leaderboard[mode][player] / 1000.0 << " ";
      } else {
        setConsoleCursorPosition(
          PADDING_LB_X + distanceToCenterOfCell(4, cellWidth[mode]) + totalWidth + 1,
          PADDING_LB_Y + headingHeight + 3 + player);
        std::cout << "____";
      }
    }

    totalWidth += 1 + cellWidth[mode];
  }
}

void setupLeaderboardDisplay() {
  PADDING_LB_X = (getWindowWidth() - leaderboardWidth) / 2;
  PADDING_LB_Y = (getWindowHeight() - leaderboardHeight - headingHeight) / 2;
}


