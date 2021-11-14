#ifndef GAME_VIEW_H_INCLUDED
#define GAME_VIEW_H_INCLUDED

#include <string>

#include "global.h"

void setupDisplay(const int &_boardWidth, const int &_boardHeight);
void displayBoard(int playerBoard[][MAX_BOARD_SIZE], const int &cursorRow,
                  const int &cursorCol, const bool &firstCall = false);
void displayNumFlags(const int &numFlags, const bool &firstCall = false);
void displayBoardStatus(const std::string &boardStatus,
                        const bool &firstCall = false);
void displayTimer(const long long &elapsedTime, const bool &firstCall = false);

#endif  // GAME_VIEW_H_INCLUDED
