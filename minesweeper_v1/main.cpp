#include "cmanip.h"
#include "global.h"
#include "main_utils.h"
#include "scene_manager.h"
#include "Leaderboard.h"

int main() {
  setupWindow();
  loadLeaderboardData();
  startSceneManager();
  return 0;
}
