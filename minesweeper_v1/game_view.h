#ifndef GAME_VIEW_H_INCLUDED
#define GAME_VIEW_H_INCLUDED

#include <string>

#include "global.h"

void setupDisplay(int boardWidth, int boardHeight);
void displayBoard(int gameBoard[][MAX_BOARD_SIZE], int cursorRow, int cursorCol,
                  bool firstCall = false);
void displayNumFlags(const int &numFlags, bool firstCall = false);
void displayBoardStatus(const std::string &boardStatus, bool firstCall = false);
void displayTimer(const long long &elapsedTime, bool firstCall = false);

#endif  // GAME_VIEW_H_INCLUDED
