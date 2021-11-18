#include "Exit.h"

#include "cmanip.h"
#include "global.h"
#include "main_utils.h"
#include "scene_manager.h"

void displayExitPrompt() {
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

        printCenteredText("Do you want to close the program?",
                          getWindowHeight() / 2 - 1);

        printCenteredText("[Y] Yes" + std::string(10, ' ') + "[N] No",
                          getWindowHeight() / 2 + 1);
      },
      CONSOLE_BACKGROUND_COLOR, RED);
}

int handleExit() {
  while (true) {
    int action = getUserAction();
    if (action == YES)
      exit(0);

    else if (action == NO || action == ESCAPE) {
      return WELCOME;
    }
  }
}

int Exit() {
  displayExitPrompt();
  return handleExit();
}
