#include "Leaderboard.h"

#include <iostream>
#include <algorithm>

#include "main_utils.h"
#include "scene_manager.h"
#include "global.h"
#include "Leaderboard.h"
#include "game_model.h"

long long leaderboard[NUMBER_OF_GAME_MODE][MAX_PLAYER_DISPLAY + 1];
bool isFirstCall = true;

void resetLeaderboard(long long leaderboard[][MAX_PLAYER_DISPLAY + 1]) {

    for (int mode = 0; mode < NUMBER_OF_GAME_MODE; mode++)
        for (int player = 0; player < MAX_PLAYER_DISPLAY; player++)
            leaderboard[mode][player] = 0;
}

void getOldLeaderboardData() {
    resetLeaderboard(leaderboard);
    transferDataToLeaderboard(leaderboard);
}

bool addToLeaderboard(int MODE, long long point, long long savedLeaderboard[][MAX_PLAYER_DISPLAY]) {
    if (isFirstCall) {
        getOldLeaderboardData();
        isFirstCall = false;
    }

    leaderboard[MODE][MAX_PLAYER_DISPLAY] = point;
    std::sort(leaderboard[MODE], leaderboard[MODE] + MAX_PLAYER_DISPLAY + 1, std::greater<long long>());

    if (point == leaderboard[MODE][MAX_PLAYER_DISPLAY]) return false;
    else {
        for (int mode = 0; mode < NUMBER_OF_GAME_MODE; mode++)
            for (int player = 0; player < MAX_PLAYER_DISPLAY; player++)
                savedLeaderboard[mode][player] = leaderboard[mode][player];
        return true;
    }
}

void displayLeaderboard() {
    for (int mode = 0; mode < NUMBER_OF_GAME_MODE; mode++) {
        for (int player = 0; player < MAX_PLAYER_DISPLAY; player++)
            if (leaderboard[mode][player] == 0) std::cout << "---- ";
            else std::cout << leaderboard[mode][player] << " ";
        std::cout << '\n';
    }
}

int Leaderboard() {
  resetConsoleScreen();
  displayLeaderboard();
  getUserAction();
  return WELCOME;
}
