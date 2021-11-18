#include "ContinueGame.h"

#include "cmanip.h"
#include "game_controller.h"
#include "global.h"
#include "main_utils.h"
#include "scene_manager.h"

void displayContinueGamePrompt() {
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
        printCenteredText("|" + std::string(40, ' ') + "|",
                          getWindowHeight() / 2 + 3);
        printCenteredText("+" + std::string(40, '=') + "+",
                          getWindowHeight() / 2 + 4);

        printCenteredText("No previously saved board found!",
                          getWindowHeight() / 2 - 1);

        printCenteredText("Do you want to start a new game?",
                          getWindowHeight() / 2);

        printCenteredText("[Y] Yes" + std::string(10, ' ') + "[N] No",
                          getWindowHeight() / 2 + 2);
      },
      CONSOLE_BACKGROUND_COLOR, YELLOW);
}

int handleContinueGame() {
  while (true) {
    int action = getUserAction();
    if (action == YES) {
      return NEW_GAME;
    } else if (action == NO || action == ESCAPE) {
      return WELCOME;
    }
  }
}

int ContinueGame() {
  if (startGame() != -1) return WELCOME;
  displayContinueGamePrompt();
  return handleContinueGame();
}
