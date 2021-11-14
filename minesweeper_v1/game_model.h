#ifndef GAME_LOGIC_H_INCLUDED
#define GAME_LOGIC_H_INCLUDED

#include <algorithm>
#include <string>
#include <vector>

#include "global.h"

enum boardStatusTypes { WIN, LOSE };

const std::vector<std::string> boardStatusOptions = {
    "Congratulation! You won!", "Umm... Quite a big explosion, right?"};

bool revealACell(GameBoard &gameBoard, const int &row, const int &col,
                 int &numOpenedCell, int totalSafeCell);
bool revealNeighboringCells(GameBoard &gameBoard, const int &row,
                            const int &col, int &numOpenedCell,
                            int totalSafeCell);
void revealAllMines(GameBoard &gameBoard, const bool &won);
void resetBoard(GameBoard &gameBoard);
bool isValidCell(const int &width, const int &height, const int &row,
                 const int &col);
void generateMineBoard(GameBoard &gameBoard, int mines);
void replaceMine(GameBoard &gameBoard, const int &row, const int &col);
std::vector<std::pair<int, int>> getNeighborsPositions(const int &width,
                                                       const int &height,
                                                       const int &row,
                                                       const int &col);
int countNeighboringCellStates(const int &width, const int &height,
                               int board[][MAX_BOARD_SIZE], const int &row,
                               const int &col, const int &cellState);
void uncoverBoard(GameBoard &gameBoard, const int &row, const int &col,
                  int &numOpenedCell);

void saveLeaderboard(const long long &elapsedTime, const int &gameLevel);
void saveBoard(GameBoard &gameBoard, const long long &elapsedTime,
               const int &numOpenedCell);
bool loadDataFile();
void updateDataFile();
void transferDataToGame(GameBoard &gameBoard, long long &elapsedTime,
                        int &numOpenedCell);
void transferDataToLeaderboard(long long leaderboard[][NUM_PLAYERS_PER_LEVEL]);

#endif  // GAME_MODEL_H_INCLUDED
