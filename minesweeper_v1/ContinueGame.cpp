#include "ContinueGame.h"

#include "main_utils.h"
#include "scene_manager.h"
#include "game_controller.h"

int ContinueGame() {
  resetConsoleScreen();
  return startGame(CONTINUE_GAME);
}
