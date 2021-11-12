#include "NewGame.h"

#include <iostream>

#include "game_controller.h"
#include "global.h"
#include "main_utils.h"
#include "scene_manager.h"

void displayLevelOptions(const int& oldLevelOption) {
  printCenteredText("SELECT A DIFFICULTY LEVEL TO CHALLENGE YOURSELF", 0);
  printCenteredText("Press [WASD] to move, [J] to select", 1);

  for (int i = 0; i < levelOptions.size(); ++i) {
    setConsoleCursorPosition(
        getStartPositionOfACenteredText(levelOptions[i].size()), 3 + i);

    if (i == oldLevelOption) {
      printColoredTextWrapper([&]() { std::cout << levelOptions[i]; },
                              CONSOLE_SELECTED_BACKGROUND_COLOR,
                              CONSOLE_SELECTED_TEXT_COLOR);
    } else
      std::cout << levelOptions[i];
  }
}

int handleGameLevelOptions(const int& oldLevelOption) {
  bool selected = false;
  int newLevelOption = oldLevelOption;
  while (!selected) {
    int action = getUserAction();
    if (action == UP) {
      setConsoleCursorPosition(
          getStartPositionOfACenteredText(levelOptions[newLevelOption].size()),
          3 + newLevelOption);

      std::cout << levelOptions[newLevelOption];

      newLevelOption = mod(newLevelOption - 1, levelOptions.size());
      printColoredTextWrapper(
          [&]() {
            setConsoleCursorPosition(getStartPositionOfACenteredText(
                                         levelOptions[newLevelOption].size()),
                                     3 + newLevelOption);
            std::cout << levelOptions[newLevelOption];
          },
          CONSOLE_SELECTED_BACKGROUND_COLOR, CONSOLE_SELECTED_TEXT_COLOR);
    } else if (action == DOWN) {
      setConsoleCursorPosition(
          getStartPositionOfACenteredText(levelOptions[newLevelOption].size()),
          3 + newLevelOption);

      std::cout << levelOptions[newLevelOption];

      newLevelOption = mod(newLevelOption + 1, levelOptions.size());

      printColoredTextWrapper(
          [&]() {
            setConsoleCursorPosition(getStartPositionOfACenteredText(
                                         levelOptions[newLevelOption].size()),
                                     3 + newLevelOption);
            std::cout << levelOptions[newLevelOption];
          },
          CONSOLE_SELECTED_BACKGROUND_COLOR, CONSOLE_SELECTED_TEXT_COLOR);
    } else if (action == MOUSE1) {
      selected = 1;
      break;
    }
  }
  return newLevelOption;
}

int NewGame() {
  static int oldLevelOption = 0;
  resetConsoleScreen();
  displayLevelOptions(oldLevelOption);
  oldLevelOption = handleGameLevelOptions(oldLevelOption);
  boardHeight = boardLevelsInfo[oldLevelOption].height;
  boardWidth = boardLevelsInfo[oldLevelOption].width;
  numMines = boardLevelsInfo[oldLevelOption].numMines;
  PADDING_X =
      (WINDOW_WIDTH - (BORDER_WIDTH + CELL_WIDTH * boardWidth + BORDER_WIDTH)) /
      2;
  PADDING_Y =
      (WINDOW_HEIGHT - (PANEL_HEIGHT + BORDER_HEIGHT +
                        CELL_HEIGHT * boardHeight + BORDER_HEIGHT + 2)) /
      2;
  return startGame(NEW);
}
