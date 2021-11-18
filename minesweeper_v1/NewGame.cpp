#include "NewGame.h"

#include <iostream>

#include "game_controller.h"
#include "global.h"
#include "main_utils.h"
#include "scene_manager.h"

int PADDING_LEVEL_X, PADDING_LEVEL_Y;

void setupLevelDisplay(int spacing) {
  PADDING_LEVEL_Y = (getWindowHeight() - spacing) / 2;
}

int getLevelPosition() { return PADDING_LEVEL_Y; }

void displayLevelHeaderAndFooter() {
  const int spacing = 1;

  std::vector<std::string> headerText = {
      R"(  ___    _    __  __  ___  __  __   ___   ___   ___ )",
      R"( / __|  /_\  |  \/  || __||  \/  | / _ \ |   \ | __|)",
      R"(| (_ | / _ \ | |\/| || _| | |\/| || (_) || |) || _| )",
      R"( \___|/_/ \_\|_|  |_||___||_|  |_| \___/ |___/ |___|)",
  };

  for (int i = 0; i < headerText.size(); i++)
    printCenteredText(headerText[i], 3 + i);

  printCenteredText("Select a difficulty level to challenge yourself!",
                    3 + headerText.size());

  printCenteredText(R"([J] Select    [Esc] Back to Menu)",
                    getWindowHeight() - 1 - 1);
}

void displayLevelOptions(const int& oldLevelOption) {
  displayLevelHeaderAndFooter();

  for (int i = 0; i < levelOptions.size(); ++i) {
    setConsoleCursorPosition(
        getStartPositionOfACenteredText(levelOptions[i].size()),
        getLevelPosition() + i);

    if (i == oldLevelOption) {
      printColoredTextWrapper([&]() { std::cout << levelOptions[i]; },
                              CONSOLE_SELECTED_BACKGROUND_COLOR,
                              CONSOLE_SELECTED_TEXT_COLOR);
    } else
      std::cout << levelOptions[i];
  }
}

int handleLevelOptions(const int& oldLevelOption) {
  bool selected = false;
  int newLevelOption = oldLevelOption;
  while (!selected) {
    int action = getUserAction();
    if (action == ESCAPE) {
      return -1;
    } else if (action == UP) {
      setConsoleCursorPosition(
          getStartPositionOfACenteredText(levelOptions[newLevelOption].size()),
          getLevelPosition() + newLevelOption);

      std::cout << levelOptions[newLevelOption];

      newLevelOption = mod(newLevelOption - 1, levelOptions.size());
      printColoredTextWrapper(
          [&]() {
            setConsoleCursorPosition(getStartPositionOfACenteredText(
                                         levelOptions[newLevelOption].size()),
                                     getLevelPosition() + newLevelOption);
            std::cout << levelOptions[newLevelOption];
          },
          CONSOLE_SELECTED_BACKGROUND_COLOR, CONSOLE_SELECTED_TEXT_COLOR);
    } else if (action == DOWN) {
      setConsoleCursorPosition(
          getStartPositionOfACenteredText(levelOptions[newLevelOption].size()),
          getLevelPosition() + newLevelOption);

      std::cout << levelOptions[newLevelOption];

      newLevelOption = mod(newLevelOption + 1, levelOptions.size());

      printColoredTextWrapper(
          [&]() {
            setConsoleCursorPosition(getStartPositionOfACenteredText(
                                         levelOptions[newLevelOption].size()),
                                     getLevelPosition() + newLevelOption);
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
  setupLevelDisplay(levelOptions.size());

  displayLevelOptions(oldLevelOption);
  int selectedLevel = handleLevelOptions(oldLevelOption);
  if (selectedLevel == -1) return WELCOME;
  oldLevelOption = selectedLevel;
  return startGame(selectedLevel);
}
