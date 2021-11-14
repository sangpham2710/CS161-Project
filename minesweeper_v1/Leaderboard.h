#ifndef LEADERBOARD_H_INCLUDED
#define LEADERBOARD_H_INCLUDED

#include "global.h"

void resetLeaderboard(long long leaderboard[][NUM_PLAYERS_PER_LEVEL]);
void getOldLeaderboardData();
bool addToLeaderboard(int MODE, long long point,
                      long long savedLeaderboard[][NUM_PLAYERS_PER_LEVEL]);
void displayLeaderboard();
int Leaderboard();
#endif  // LEADERBOARD_H_INCLUDED
