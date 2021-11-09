#include "NewGame.h"

#include <iostream>

#include "game_controller.h"
#include "global.h"
#include "main_utils.h"
#include "scene_manager.h"

int NewGame() {
  resetConsoleScreen();
  std::cout << "Enter width, height, the amound of mines: ";
  std::cin >> boardWidth >> boardHeight >> numMines;
  PADDING_X =
      (WINDOW_WIDTH - (BORDER_WIDTH + CELL_WIDTH * boardWidth + BORDER_WIDTH)) /
      2;
  PADDING_Y =
      (WINDOW_HEIGHT - (PANEL_HEIGHT + BORDER_HEIGHT +
                        CELL_HEIGHT * boardHeight + BORDER_HEIGHT + 2)) /
      2;
  return startGame();
}