#include "ContinueGame.h"

#include "game_controller.h"
#include "main_utils.h"
#include "scene_manager.h"

int ContinueGame() {
  resetConsoleScreen();
  return startGame(CONTINUE);
}
