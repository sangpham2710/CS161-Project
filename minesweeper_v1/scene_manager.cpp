#include "scene_manager.h"

#include "ContinueGame.h"
#include "Exit.h"
#include "Leaderboard.h"
#include "NewGame.h"
#include "Welcome.h"
#include "instructions.h"

void startSceneManager() {
  static int currentScene = WELCOME;
  while (true) {
    switch (currentScene) {
      case NEW_GAME:
        currentScene = NewGame();
        break;
      case CONTINUE_GAME:
        currentScene = ContinueGame();
        break;
      case INSTRUCTION:
        currentScene = Instructions();
        break;
      case LEADERBOARD:
        currentScene = Leaderboard();
        break;
      case EXIT:
        currentScene = Exit();
        break;
      case WELCOME:
        currentScene = Welcome();
        break;
    }
  }
}
