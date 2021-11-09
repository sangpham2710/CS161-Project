#include "Welcome.h"

#include <iostream>

#include "cmanip.h"
#include "global.h"
#include "main_utils.h"

void displayWelcomeOptions(const int& oldWelcomeOption) {
  printCenteredText("WELCOME TO MINESWEEPER V1.0", 0);
  printCenteredText("Press [WASD] to move, [J] to select", 1);

  for (int i = 0; i < welcomeOptions.size(); ++i) {
    setConsoleCursorPosition(
        getStartPositionOfACenteredText(welcomeOptions[i].size()), 3 + i);

    if (i == oldWelcomeOption) {
      printColoredTextWrapper([&]() { std::cout << welcomeOptions[i]; },
                              CONSOLE_SELECTED_BACKGROUND_COLOR,
                              CONSOLE_SELECTED_TEXT_COLOR);
    } else
      std::cout << welcomeOptions[i];
  }
}

int handleWelcomeOptions(const int& oldWelcomeOption) {
  bool selected = false;
  int newWelcomeOption = oldWelcomeOption;
  while (!selected) {
    int action = getUserAction();
    if (action == UP) {
      setConsoleCursorPosition(getStartPositionOfACenteredText(
                                   welcomeOptions[newWelcomeOption].size()),
                               3 + newWelcomeOption);

      std::cout << welcomeOptions[newWelcomeOption];

      newWelcomeOption = mod(newWelcomeOption - 1, welcomeOptions.size());
      printColoredTextWrapper(
          [&]() {
            setConsoleCursorPosition(
                getStartPositionOfACenteredText(
                    welcomeOptions[newWelcomeOption].size()),
                3 + newWelcomeOption);
            std::cout << welcomeOptions[newWelcomeOption];
          },
          CONSOLE_SELECTED_BACKGROUND_COLOR, CONSOLE_SELECTED_TEXT_COLOR);
    } else if (action == DOWN) {
      setConsoleCursorPosition(getStartPositionOfACenteredText(
                                   welcomeOptions[newWelcomeOption].size()),
                               3 + newWelcomeOption);

      std::cout << welcomeOptions[newWelcomeOption];

      newWelcomeOption = mod(newWelcomeOption + 1, welcomeOptions.size());

      printColoredTextWrapper(
          [&]() {
            setConsoleCursorPosition(
                getStartPositionOfACenteredText(
                    welcomeOptions[newWelcomeOption].size()),
                3 + newWelcomeOption);
            std::cout << welcomeOptions[newWelcomeOption];
          },
          CONSOLE_SELECTED_BACKGROUND_COLOR, CONSOLE_SELECTED_TEXT_COLOR);
    } else if (action == MOUSE1) {
      selected = 1;
      break;
    }
  }
  return newWelcomeOption;
}

int Welcome() {
  static int oldWelcomeOption = 0;
  resetConsoleScreen();
  displayWelcomeOptions(oldWelcomeOption);
  oldWelcomeOption = handleWelcomeOptions(oldWelcomeOption);
  return oldWelcomeOption;
}
