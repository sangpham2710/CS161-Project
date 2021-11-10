#include "GameLevel.h"

#include <iostream>

#include "cmanip.h"
#include "global.h"
#include "main_utils.h"
#include "scene_manager.h"
#include "NewGame.h"

void displayGameLevelOptions(const int& oldGameLevelOption) {
  printCenteredText("SELECT A DIFFICULTY LEVEL TO CHALLENGE YOURSELF", 0);
  printCenteredText("Press [WASD] to move, [J] to select", 1);

  for (int i = 0; i < GameLevelOptions.size(); ++i) {
    setConsoleCursorPosition(
        getStartPositionOfACenteredText(GameLevelOptions[i].size()), 3 + i);

    if (i == oldGameLevelOption) {
      printColoredTextWrapper([&]() { std::cout << GameLevelOptions[i]; },
                              CONSOLE_SELECTED_BACKGROUND_COLOR,
                              CONSOLE_SELECTED_TEXT_COLOR);
    } else
      std::cout << GameLevelOptions[i];
  }
}

int handleGameLevelOptions(const int& oldGameLevelOption) {
  bool selected = false;
  int newGameLevelOption = oldGameLevelOption;
  while (!selected) {
    int action = getUserAction();
    if (action == UP) {
      setConsoleCursorPosition(getStartPositionOfACenteredText(
                                   GameLevelOptions[newGameLevelOption].size()),
                               3 + newGameLevelOption);

      std::cout << GameLevelOptions[newGameLevelOption];

      newGameLevelOption = mod(newGameLevelOption - 1, GameLevelOptions.size());
      printColoredTextWrapper(
          [&]() {
            setConsoleCursorPosition(
                getStartPositionOfACenteredText(
                    GameLevelOptions[newGameLevelOption].size()),
                3 + newGameLevelOption);
            std::cout << GameLevelOptions[newGameLevelOption];
          },
          CONSOLE_SELECTED_BACKGROUND_COLOR, CONSOLE_SELECTED_TEXT_COLOR);
    } else if (action == DOWN) {
      setConsoleCursorPosition(getStartPositionOfACenteredText(
                                   GameLevelOptions[newGameLevelOption].size()),
                               3 + newGameLevelOption);

      std::cout << GameLevelOptions[newGameLevelOption];

      newGameLevelOption = mod(newGameLevelOption + 1, GameLevelOptions.size());

      printColoredTextWrapper(
          [&]() {
            setConsoleCursorPosition(
                getStartPositionOfACenteredText(
                    GameLevelOptions[newGameLevelOption].size()),
                3 + newGameLevelOption);
            std::cout << GameLevelOptions[newGameLevelOption];
          },
          CONSOLE_SELECTED_BACKGROUND_COLOR, CONSOLE_SELECTED_TEXT_COLOR);
    } else if (action == MOUSE1) {
      selected = 1;
      break;
    }
  }
  return newGameLevelOption;
}

int GameLevel() {
  static int oldGameLevelOption = 0;
  resetConsoleScreen();
  displayGameLevelOptions(oldGameLevelOption);
  oldGameLevelOption = handleGameLevelOptions(oldGameLevelOption);
  boardHeight = boardInfo[oldGameLevelOption].HEIGHT;
  boardWidth = boardInfo[oldGameLevelOption].WIDTH;
  numMines = boardInfo[oldGameLevelOption].MINES;
  return NewGame();
}

