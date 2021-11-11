#ifndef USER_DATA_H_INCLUDED
#define USER_DATA_H_INCLUDED

#include "global.h"

void addToLeaderboard(const long long &elapsedTime, const int &gameLevel);
void saveBoard(const int &WIDTH, const int &HEIGHT, const int &MINES, const int &flagLeft,
               const long long &elapsedTime, const int &totalSafelyOpenedCell,
               int gameBoard[][MAX_BOARD_SIZE], int mineBoard[][MAX_BOARD_SIZE]);
void loadDataFile();
void updateDataFile();
void transferDataToGame(int &flagLeft, long long &elapsedTime, int &totalSafelyOpenedCell,
                        int gameBoard[][MAX_BOARD_SIZE], int mineBoard[][MAX_BOARD_SIZE]);

#endif // USER_DATA_H_INCLUDED
