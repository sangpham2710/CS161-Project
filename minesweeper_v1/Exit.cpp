#include "Exit.h"

#include "cmanip.h"
#include "main_utils.h"
#include "scene_manager.h"

void displayExitPrompt() {
  printCenteredText("Do you want to close the program ?", 9);
  printCenteredText("[Y] Yes / [N] No", 10);
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
