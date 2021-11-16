#ifndef LEADERBOARD_H_INCLUDED
#define LEADERBOARD_H_INCLUDED

#include "global.h"

void resetLeaderboardFile();
void loadLeaderboardData();
void updateLeaderboardData();
bool addToLeaderboard(const int& level, const long long& elapsedTime);
void resetLeaderboard();
void displayLeaderboard();
int Leaderboard();
void setupLeaderboardDisplay();

#endif  // LEADERBOARD_H_INCLUDED
