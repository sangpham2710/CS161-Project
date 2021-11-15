#ifndef LEADERBOARD_H_INCLUDED
#define LEADERBOARD_H_INCLUDED

#include "global.h"

void resetLeaderboard(long long leaderboard[][NUM_PLAYERS_PER_LEVEL + 1]);
void getOldLeaderboardData();
bool addToLeaderboard(const int& level, const long long& elapsedTime,
                      long long savedLeaderboard[][NUM_PLAYERS_PER_LEVEL + 1]);
void displayLeaderboard();
int Leaderboard();

#endif  // LEADERBOARD_H_INCLUDED
