#ifndef LEADERBOARD_H_INCLUDED
#define LEADERBOARD_H_INCLUDED

#include "global.h"

void resetLeaderboard(long long leaderboard[][MAX_PLAYER_DISPLAY + 1]);
void getOldLeaderboardData();
bool addToLeaderboard(int MODE, long long point, long long savedLeaderboard[][MAX_PLAYER_DISPLAY]);
void displayLeaderboard();
int Leaderboard();
#endif  // LEADERBOARD_H_INCLUDED
