#ifndef LEADERBOARD_H_INCLUDED
#define LEADERBOARD_H_INCLUDED

#include "global.h"

void initLeaderboardFile();
void loadLeaderboardData();
void updateLeaderboardData();
bool addToLeaderboard(const int& level, const long long& elapsedTime);
void resetLeaderboard();
void displayLeaderboard();
int Leaderboard();

#endif  // LEADERBOARD_H_INCLUDED
