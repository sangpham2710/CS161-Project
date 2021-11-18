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

int PADDING_LEADERBOARD_X, PADDING_LEADERBOARD_Y;
const int leaderboardWidth =
              strlen("+------------+--------------+------------+"),
          leaderboardHeight = 14;

// 3 che do
// 0 0 0 0 0 0 0 0 0 0
// 0 0 0 0 0 0 0 0 0 0
// 0 0 0 0 0 0 0 0 0 0
long long leaderboard[NUM_LEVELS][NUM_PLAYERS_PER_LEVEL + 1];

//{
//    R"()",
//    R"()",
//    R"()",
//    R"()",
//  };

std::vector<std::string> getLeaderboardHeaderText() {
  return {
    R"(  _     ___    _    ___   ___  ___  ___   ___    _    ___  ___  )",
    R"( | |   | __|  /_\  |   \ | __|| _ \| _ ) / _ \  /_\  | _ \|   \ )",
    R"( | |__ | __| / _ \ | |) || __||   /| _ \| (_) |/ _ \ |   /| |) |)",
    R"( |____||___|/_/ \_\|___/ |___||_|_\|___/ \___/ _/ \_\|_|_\|___/ )",
  };
}

int getLeaderboardPosition() {
    return PADDING_LEADERBOARD_Y + 1;
}

void displayLeaderboardHeaderAndFooter() {
  std::vector<std::string> headerText = getLeaderboardHeaderText();
  const int spacing = 1;

  for (int i = 0; i < headerText.size(); i++)
    printCenteredText(headerText[i], PADDING_LEADERBOARD_Y - headerText.size() + i);

  //printCenteredText("Version 1.0", PADDING_LEADERBOARD_Y - spacing);
  printCenteredText(R"([J] Back to Menu    [R] Reset Leaderboard)",
                    getWindowHeight() - 1 - 1);
}

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

int addToLeaderboard(const int& level, const long long& elapsedTime) {
  loadLeaderboardData();
  leaderboard[level][NUM_PLAYERS_PER_LEVEL] = elapsedTime;
  std::sort(leaderboard[level], leaderboard[level] + NUM_PLAYERS_PER_LEVEL + 1);

  if (elapsedTime != leaderboard[level][NUM_PLAYERS_PER_LEVEL]) {
    updateLeaderboardData();
    for (int player = 0; player < NUM_PLAYERS_PER_LEVEL; player++)
        if (leaderboard[level][player] == elapsedTime) return player + 1;
  } else
    return -1;
}

void displayResetLeaderboard() {
  printColoredTextWrapper(
      [&]() {
        printCenteredText("+" + std::string(40, '=') + "+",
                          getWindowHeight() / 2 - 3);
        printCenteredText("|" + std::string(40, ' ') + "|",
                          getWindowHeight() / 2 - 2);
        printCenteredText("|" + std::string(40, ' ') + "|",
                          getWindowHeight() / 2 - 1);
        printCenteredText("|" + std::string(40, ' ') + "|",
                          getWindowHeight() / 2);
        printCenteredText("|" + std::string(40, ' ') + "|",
                          getWindowHeight() / 2 + 1);
        printCenteredText("|" + std::string(40, ' ') + "|",
                          getWindowHeight() / 2 + 2);
        printCenteredText("+" + std::string(40, '=') + "+",
                          getWindowHeight() / 2 + 3);

        printCenteredText("You sure? (all data will be lost!)",
                          getWindowHeight() / 2 - 1);

        printCenteredText("[Y] Yes" + std::string(10, ' ') + "[N] No",
                          getWindowHeight() / 2 + 1);
      },
      CONSOLE_BACKGROUND_COLOR, RED);
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

int distanceToCenterOfCell(const int& textSize, const int& cellWidth) {
  return (cellWidth - textSize) / 2;
}

void displayLeaderboard() {
  int cellWidth[NUM_LEVELS] = {12, 14, 12};

  loadLeaderboardData();
  resetConsoleScreen();

  displayLeaderboardHeaderAndFooter();

  setConsoleCursorPosition(PADDING_LEADERBOARD_X, getLeaderboardPosition());
  std::cout << "+" + std::string(12, '=') + "+" + std::string(14, '=') + "+" +
                   std::string(12, '=') + "+"
            << '\n';

  setConsoleCursorPosition(PADDING_LEADERBOARD_X, getLeaderboardPosition() + 1);
  std::cout << "|  Beginner  | Intermediate |   Expert   |" << '\n';

  setConsoleCursorPosition(PADDING_LEADERBOARD_X, getLeaderboardPosition() + 2);
  std::cout << "+" + std::string(12, '=') + "+" + std::string(14, '=') + "+" +
                   std::string(12, '=') + "+"
            << '\n';

  for (int i = 0; i < NUM_PLAYERS_PER_LEVEL; i++) {
    setConsoleCursorPosition(PADDING_LEADERBOARD_X,
                             getLeaderboardPosition() + 3 + i);
    std::cout << "|            |              |            |" << '\n';
  }

  setConsoleCursorPosition(
      PADDING_LEADERBOARD_X, getLeaderboardPosition() + 3 + NUM_PLAYERS_PER_LEVEL);
  std::cout << "+" + std::string(12, '=') + "+" + std::string(14, '=') + "+" +
                   std::string(12, '=') + "+"
            << '\n';

  int totalWidth = 0;
  for (int mode = 0; mode < NUM_LEVELS; mode++) {
    for (int player = 0; player < NUM_PLAYERS_PER_LEVEL; player++) {
      if (leaderboard[mode][player] != MAX_TIME) {
        setConsoleCursorPosition(
            PADDING_LEADERBOARD_X +
                distanceToCenterOfCell(
                    intToString(leaderboard[mode][player]).size() + 1,
                    cellWidth[mode]) +
                totalWidth + 1,
            getLeaderboardPosition() + 3 + player);

        std::cout << std::fixed << std::setprecision(3)
                  << leaderboard[mode][player] / 1000.0 << " ";
      } else {
        setConsoleCursorPosition(
            PADDING_LEADERBOARD_X + distanceToCenterOfCell(4, cellWidth[mode]) +
                totalWidth + 1,
            getLeaderboardPosition() + 3 + player);
        std::cout << "____";
      }
    }

    totalWidth += 1 + cellWidth[mode];
  }
}

void setupLeaderboardDisplay() {
  PADDING_LEADERBOARD_X = (getWindowWidth() - leaderboardWidth) / 2;
  PADDING_LEADERBOARD_Y = (getWindowHeight() - leaderboardHeight) / 2;
}
