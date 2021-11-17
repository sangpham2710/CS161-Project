#include "Welcome.h"

#include <iostream>
#include <vector>

#include "cmanip.h"
#include "global.h"
#include "main_utils.h"
#include "windows.h"

int WELCOME_PADDING_X, WELCOME_PADDING_Y;
const std::string backplash(1, (char)92);

void setupWelcomeDisplay(int headerHeight, int option, int spacing) {
  WELCOME_PADDING_Y = (getWindowHeight() - headerHeight - spacing - option) / 2;
}

//    std::vector<std::string> headerText = {
//    "",
//    "",
//    "",
//    "",
//    };

std::vector<std::string> getHeaderText() {
  return {
    "  __  __ ___ _  _ ___ _____      _____ ___ ___ ___ ___  ",
    " |  " + backplash + "/  |_ _| " + backplash + "| | __/ __" + backplash + " " + backplash + "    / / __| __| _ " + backplash + " __| _ " + backplash + " ",
    " | |" + backplash + "/| || || .` | _|" + backplash + "__ " + backplash + backplash + " " + backplash + "/" + backplash + "/ /| _|| _||  _/ _||   / ",
    " |_|  |_|___|_|" + backplash + "_|___|___/ " + backplash + "_/" + backplash + "_/ |___|___|_| |___|_|_" + backplash + " ",
  };
}

void displayWelcomeOptions(const int& oldWelcomeOption) {
  std::vector<std::string> headerText = getHeaderText();

  setupWelcomeDisplay(headerText.size() + 2, welcomeOptions.size(), 2);

  for (int i = 0; i < headerText.size(); i++)
    printCenteredText(headerText[i], WELCOME_PADDING_Y + i);

  printCenteredText("Version 1.0", WELCOME_PADDING_Y + headerText.size());
  printCenteredText("Press [WASD] to move, [J] to select",
                    WELCOME_PADDING_Y + 1 + headerText.size());

  for (int i = 0; i < welcomeOptions.size(); ++i) {
    setConsoleCursorPosition(
      getStartPositionOfACenteredText(welcomeOptions[i].size()),
      WELCOME_PADDING_Y + 3 + headerText.size() + i);

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
                               WELCOME_PADDING_Y + 3 + headerText.size() + newWelcomeOption);

      std::cout << welcomeOptions[newWelcomeOption];

      newWelcomeOption = mod(newWelcomeOption - 1, welcomeOptions.size());
      printColoredTextWrapper(
      [&]() {
        setConsoleCursorPosition(
          getStartPositionOfACenteredText(
            welcomeOptions[newWelcomeOption].size()),
          WELCOME_PADDING_Y + 3 + headerText.size() + newWelcomeOption);
        std::cout << welcomeOptions[newWelcomeOption];
      },
      CONSOLE_SELECTED_BACKGROUND_COLOR, CONSOLE_SELECTED_TEXT_COLOR);
    } else if (action == DOWN) {
      setConsoleCursorPosition(getStartPositionOfACenteredText(
                                 welcomeOptions[newWelcomeOption].size()),
                               WELCOME_PADDING_Y + 3 + headerText.size() +  + newWelcomeOption);

      std::cout << welcomeOptions[newWelcomeOption];

      newWelcomeOption = mod(newWelcomeOption + 1, welcomeOptions.size());

      printColoredTextWrapper(
      [&]() {
        setConsoleCursorPosition(
          getStartPositionOfACenteredText(
            welcomeOptions[newWelcomeOption].size()),
          WELCOME_PADDING_Y + 3 + headerText.size() +  + newWelcomeOption);
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
