#ifndef GAME_LOGIC_H_INCLUDED
#define GAME_LOGIC_H_INCLUDED

#include <algorithm>
#include <string>
#include <vector>

#include "global.h"

enum boardStatusTypes { WIN, LOSE };

const std::vector<std::string> boardStatusOptions = {
    "Congratulation! You won!", "Umm... Quite a big explosion, right?"};

bool revealACell(int gameBoard[][MAX_BOARD_SIZE],
                 int mineBoard[][MAX_BOARD_SIZE], const int &row,
                 const int &col, int &totalSafelyOpenedCell, int totalSafeCell,
                 std::string &boardStatus);
bool revealNeighboringCells(int gameBoard[][MAX_BOARD_SIZE],
                            int mineBoard[][MAX_BOARD_SIZE], const int &row,
                            const int &col, int &totalSafelyOpenedCell,
                            int totalSafeCell, std::string &boardStatus);
void revealAllMines(int gameBoard[][MAX_BOARD_SIZE],
                    int mineBoard[][MAX_BOARD_SIZE], const bool &won);
void resetBoard(int gameBoard[][MAX_BOARD_SIZE],
                int mineBoard[][MAX_BOARD_SIZE]);
bool isValidCell(const int &row, const int &col);
void generateMineBoard(int mineBoard[][MAX_BOARD_SIZE], int mines);
void replaceMine(int mineBoard[][MAX_BOARD_SIZE], const int &row,
                 const int &col);
std::vector<std::pair<int, int>> getNeighborsPositions(const int &row,
                                                       const int &col);
int countNeighboringCellStates(int board[][MAX_BOARD_SIZE], const int &row,
                               const int &col, const int &cellState);
void uncoverBoard(int gameBoard[][MAX_BOARD_SIZE],
                  int mineBoard[][MAX_BOARD_SIZE], const int &row,
                  const int &col, int &totalSafelyOpenedCell);

void saveLeaderboard(const long long &elapsedTime, const int &gameLevel);
void saveBoard(const int &WIDTH, const int &HEIGHT, const int &MINES,
               const int &flagLeft, const long long &elapsedTime,
               const int &totalSafelyOpenedCell,
               int gameBoard[][MAX_BOARD_SIZE],
               int mineBoard[][MAX_BOARD_SIZE]);
bool loadDataFile();
void updateDataFile();
void transferDataToGame(int &flagLeft, long long &elapsedTime,
                        int &totalSafelyOpenedCell,
                        int gameBoard[][MAX_BOARD_SIZE],
                        int mineBoard[][MAX_BOARD_SIZE]);
void transferDataToLeaderboard(
    long long leaderboard[][NUM_RECORDS_PER_LEVEL + 1]);

#endif  // GAME_MODEL_H_INCLUDED
