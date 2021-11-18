#include "Welcome.h"

#include <iostream>
#include <vector>

#include "cmanip.h"
#include "global.h"
#include "main_utils.h"
#include "windows.h"

int WELCOME_PADDING_X, WELCOME_PADDING_Y;

void setupWelcomeDisplay(int option) {
  WELCOME_PADDING_Y = (getWindowHeight() - option) / 2;
}

//{
//    R"()",
//    R"()",
//    R"()",
//    R"()",
//  };

std::vector<std::string> getHeaderText() {
  return {
    R"(  __  __  ___  _  _  ___  ___ __      __ ___  ___  ___  ___  ___ )",
    R"( |  \/  ||_ _|| \| || __|/ __|\ \    / /| __|| __|| _ \| __|| _ \)",
    R"( | |\/| | | | | .` || __|\__ \ \ \/\/ / | __|| __||  _/| __||   /)",
    R"( |_|  |_||___||_|\_||___||___/  \_/\_/  |___||___||_|  |___||_|_\)",
  };
}

int getPosition() {
    return WELCOME_PADDING_Y + 1;
}

void displayWelcomeOptions(const int& oldWelcomeOption) {
  setupWelcomeDisplay(welcomeOptions.size());

  std::vector<std::string> headerText = getHeaderText();
  const int spacing = 1;


  for (int i = 0; i < headerText.size(); i++)
    printCenteredText(headerText[i], 3 + i);

  printCenteredText("Version 1.0", 3 + headerText.size());
  printCenteredText("[WASD] Move     [J] Select",
                    getWindowHeight() - 1 - 1);


  for (int i = 0; i < welcomeOptions.size(); ++i) {
    setConsoleCursorPosition(
      getStartPositionOfACenteredText(welcomeOptions[i].size()),
      getPosition() + i);

    if (i == oldWelcomeOption) {
      printColoredTextWrapper([&]() {
        std::cout << welcomeOptions[i];
      },
      CONSOLE_SELECTED_BACKGROUND_COLOR,
      CONSOLE_SELECTED_TEXT_COLOR);
    } else
      std::cout << welcomeOptions[i];
  }
}

int handleWelcomeOptions(const int& oldWelcomeOption) {
  std::vector <std::string> headerText = getHeaderText();
  bool selected = false;
  int newWelcomeOption = oldWelcomeOption;
  while (!selected) {
    int action = getUserAction();
    if (action == UP) {
      setConsoleCursorPosition(getStartPositionOfACenteredText(
                                 welcomeOptions[newWelcomeOption].size()),
                               getPosition() + newWelcomeOption);

      std::cout << welcomeOptions[newWelcomeOption];

      newWelcomeOption = mod(newWelcomeOption - 1, welcomeOptions.size());
      printColoredTextWrapper(
      [&]() {
        setConsoleCursorPosition(
          getStartPositionOfACenteredText(
            welcomeOptions[newWelcomeOption].size()),
          getPosition() + newWelcomeOption);
        std::cout << welcomeOptions[newWelcomeOption];
      },
      CONSOLE_SELECTED_BACKGROUND_COLOR, CONSOLE_SELECTED_TEXT_COLOR);
    } else if (action == DOWN) {
      setConsoleCursorPosition(getStartPositionOfACenteredText(
                                 welcomeOptions[newWelcomeOption].size()),
                               getPosition() +  + newWelcomeOption);

      std::cout << welcomeOptions[newWelcomeOption];

      newWelcomeOption = mod(newWelcomeOption + 1, welcomeOptions.size());

      printColoredTextWrapper(
      [&]() {
        setConsoleCursorPosition(
          getStartPositionOfACenteredText(
            welcomeOptions[newWelcomeOption].size()),
          getPosition() +  + newWelcomeOption);
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
