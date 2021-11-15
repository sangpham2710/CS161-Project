#ifndef GAME_LOGIC_H_INCLUDED
#define GAME_LOGIC_H_INCLUDED

#include <algorithm>
#include <string>
#include <vector>

#include "global.h"

enum boardStatusTypes { WIN, LOSE };

const std::vector<std::string> boardStatusOptions = {
    "Congratulation! You won!", "Umm... Quite a big explosion, right?"};

void constructBoard(GameBoard &gameBoard, const int &currentLevel);
void copyBoard(GameBoard &dest, const GameBoard &src);
bool revealACell(GameBoard &gameBoard, const int &row, const int &col);
bool revealNeighboringCells(GameBoard &gameBoard, const int &row,
                            const int &col);
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
void uncoverBoard(GameBoard &gameBoard, const int &row, const int &col);

void saveLeaderboard(const int &gameLevel, const long long &elapsedTime);
void saveBoard(GameBoard &gameBoard);
bool loadDataFile();
void updateDataFile();
void transferDataToGame(GameBoard &gameBoard);
void transferDataToLeaderboard(
    long long leaderboard[][NUM_PLAYERS_PER_LEVEL + 1]);

#endif  // GAME_MODEL_H_INCLUDED
