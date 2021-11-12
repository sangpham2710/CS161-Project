#include "game_model.h"

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include "Leaderboard.h"

std::mt19937 randInt(
    std::chrono::steady_clock::now().time_since_epoch().count());

bool revealACell(int gameBoard[][MAX_BOARD_SIZE],
                 int mineBoard[][MAX_BOARD_SIZE], const int &row,
                 const int &col, int &totalSafelyOpenedCell, int totalSafeCell,
                 std::string &boardStatus) {
  if (mineBoard[row][col] == UNKNOWN) {
    uncoverBoard(gameBoard, mineBoard, row, col, totalSafelyOpenedCell);
    if (totalSafelyOpenedCell == totalSafeCell) {
      revealAllMines(gameBoard, mineBoard, true);
      boardStatus = boardStatusOptions[WIN];
      return true;
    }
  } else if (mineBoard[row][col] == MINE) {
    gameBoard[row][col] = MINE;
    revealAllMines(gameBoard, mineBoard, false);
    boardStatus = boardStatusOptions[LOSE];
    return true;
  }

  return false;
}

// ACTION: Reveal Neighbor Cells
bool revealNeighboringCells(int gameBoard[][MAX_BOARD_SIZE],
                            int mineBoard[][MAX_BOARD_SIZE], const int &row,
                            const int &col, int &totalSafelyOpenedCell,
                            int totalSafeCell, std::string &boardStatus) {
  std::vector<std::pair<int, int>> neighborsPositions =
      getNeighborsPositions(row, col);

  int flagCount = countNeighboringCellStates(gameBoard, row, col, FLAGGED);
  int mineCount = countNeighboringCellStates(mineBoard, row, col, MINE);

  if (flagCount == mineCount) {
    for (int i = 0; i < neighborsPositions.size(); i++)
      if (gameBoard[neighborsPositions[i].first]
                   [neighborsPositions[i].second] == UNKNOWN ||
          gameBoard[neighborsPositions[i].first]
                   [neighborsPositions[i].second] == QUESTIONED) {
        if (revealACell(gameBoard, mineBoard, neighborsPositions[i].first,
                        neighborsPositions[i].second, totalSafelyOpenedCell,
                        totalSafeCell, boardStatus))
          return true;
      }
  }
  return false;
}

// Clean the board for new play.
void resetBoard(int gameBoard[][MAX_BOARD_SIZE],
                int mineBoard[][MAX_BOARD_SIZE]) {
  for (int row = 0; row < boardHeight; ++row)
    for (int col = 0; col < boardWidth; ++col)
      gameBoard[row][col] = mineBoard[row][col] = UNKNOWN;
  return;
}

// Check whether the cell [row, column] is in the board or not.
bool isValidCell(const int &row, const int &col) {
  return (row >= 0) && (row < boardHeight) && (col >= 0) && (col < boardWidth);
}

// Obviously placing down mines :V
// HAVEN'T CHECKED THE CASE WE HAVE TO GUESS
void generateMineBoard(int mineBoard[][MAX_BOARD_SIZE], int mines) {
  int placed = 0;
  while (placed < mines) {
    int row = randInt() % boardHeight;
    int col = randInt() % boardWidth;
    if (mineBoard[row][col] == MINE) continue;  // already a mine
    mineBoard[row][col] = MINE;
    placed++;
  }
}

// First time hit the mine, then move it to somewhere else
void replaceMine(int mineBoard[][MAX_BOARD_SIZE], const int &row,
                 const int &col) {
  generateMineBoard(mineBoard, 1);  // add a new mine
  mineBoard[row][col] = UNKNOWN;    // remove the old one
}

// Get the valid neighbor cells
std::vector<std::pair<int, int>> getNeighborsPositions(const int &row,
                                                       const int &col) {
  std::vector<std::pair<int, int>> neighborsPositions;

  for (int dx = -1; dx <= 1; dx++)
    for (int dy = -1; dy <= 1; dy++)
      if (dx != 0 || dy != 0)
        if (isValidCell(row + dx, col + dy))
          neighborsPositions.push_back({row + dx, col + dy});

  return neighborsPositions;
}

int countNeighboringCellStates(int board[][MAX_BOARD_SIZE], const int &row,
                               const int &col, const int &cellState) {
  std::vector<std::pair<int, int>> neighborsPositions =
      getNeighborsPositions(row, col);

  int cellCount = 0;
  for (int i = 0; i < neighborsPositions.size(); i++)
    if (board[neighborsPositions[i].first][neighborsPositions[i].second] ==
        cellState)
      cellCount++;

  return cellCount;
}

void uncoverBoard(int gameBoard[][MAX_BOARD_SIZE],
                  int mineBoard[][MAX_BOARD_SIZE], const int &row,
                  const int &col, int &totalSafelyOpenedCell) {
  totalSafelyOpenedCell++;
  int mineCount = countNeighboringCellStates(mineBoard, row, col, MINE);
  gameBoard[row][col] = BLANK + mineCount;

  if (mineCount == 0) {
    std::vector<std::pair<int, int>> neighborsPositions =
        getNeighborsPositions(row, col);

    for (int i = 0; i < neighborsPositions.size(); i++)
      if (gameBoard[neighborsPositions[i].first]
                   [neighborsPositions[i].second] == UNKNOWN)
        uncoverBoard(gameBoard, mineBoard, neighborsPositions[i].first,
                     neighborsPositions[i].second, totalSafelyOpenedCell);
  }
}

// Show all the mines when the game finished
void revealAllMines(int gameBoard[][MAX_BOARD_SIZE],
                    int mineBoard[][MAX_BOARD_SIZE], const bool &won) {
  for (int row = 0; row < boardHeight; row++) {
    for (int col = 0; col < boardWidth; col++) {
      if (gameBoard[row][col] == UNKNOWN && mineBoard[row][col] == MINE) {
        if (won)
          gameBoard[row][col] = FLAGGED;
        else
          gameBoard[row][col] = MINE;
      }
    }
  }

}

long long savedTime, savedLeaderboard[NUMBER_OF_GAME_MODE][MAX_PLAYER_DISPLAY];
int savedGameLevel, savedWIDTH, savedHEIGHT, savedMINES, savedFlagLeft,
    savedOpenedCell, savedGameBoard[MAX_BOARD_SIZE][MAX_BOARD_SIZE],
    savedMineBoard[MAX_BOARD_SIZE][MAX_BOARD_SIZE];

const std::string FILENAME = "data.txt";

void saveLeaderboard(const long long &elapsedTime, const int &gameLevel) {
    if (addToLeaderboard(gameLevel, elapsedTime, savedLeaderboard)) updateDataFile();
}

void saveBoard(const int &WIDTH, const int &HEIGHT, const int &MINES,
               const int &flagLeft, const long long &elapsedTime,
               const int &totalSafelyOpenedCell,
               int gameBoard[][MAX_BOARD_SIZE],
               int mineBoard[][MAX_BOARD_SIZE]) {
  savedWIDTH = WIDTH;
  savedHEIGHT = HEIGHT;
  savedMINES = MINES;
  savedFlagLeft = flagLeft;
  savedGameLevel = curMode;
  savedTime = elapsedTime;
  savedOpenedCell = totalSafelyOpenedCell;

  for (int row = 0; row < HEIGHT; row++)
    for (int col = 0; col < WIDTH; col++) {
      savedGameBoard[row][col] = gameBoard[row][col];
      savedMineBoard[row][col] = mineBoard[row][col];
    }
  updateDataFile();
}

// DATA FILE TEMPLATE
// 3 che do
// 0 0 0 0 0 0 0 0 0 0
// 0 0 0 0 0 0 0 0 0 0
// 0 0 0 0 0 0 0 0 0 0
// gamelevel width height mine flag time total_safely_opened_cell
// 0 0 0 0 0 0
//[displayBoard]
//[mineBoard]
void updateDataFile() {
  std::ofstream dataFile(FILENAME);
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 10; j++) dataFile << savedLeaderboard[i][j] << " ";
    dataFile << '\n';
  }

  dataFile << savedGameLevel << " " << savedWIDTH << " " << savedHEIGHT << " " << savedMINES << " "
           << savedFlagLeft << " " << savedTime << " " << savedOpenedCell;
  for (int row = 0; row < savedHEIGHT; row++) {
    dataFile << '\n';
    for (int col = 0; col < savedWIDTH; col++)
      dataFile << savedGameBoard[row][col] << " ";
  }
  for (int row = 0; row < savedHEIGHT; row++) {
    dataFile << '\n';
    for (int col = 0; col < savedWIDTH; col++)
      dataFile << savedMineBoard[row][col] << " ";
  }

  dataFile.close();
}

bool loadDataFile() {
  std::ifstream dataFile(FILENAME);
  if (dataFile) return false;
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 10; j++) dataFile >> savedLeaderboard[i][j];

  dataFile >> savedGameLevel >> savedWIDTH >> savedHEIGHT >> savedMINES >> savedFlagLeft >>
      savedTime >> savedOpenedCell;
    if (savedWIDTH == 0) return false;
  for (int row = 0; row < savedHEIGHT; row++)
    for (int col = 0; col < savedWIDTH; col++)
      dataFile >> savedGameBoard[row][col];

  for (int row = 0; row < savedHEIGHT; row++)
    for (int col = 0; col < savedWIDTH; col++)
      dataFile >> savedMineBoard[row][col];

  dataFile.close();
}

void transferDataToGame(int &flagLeft, long long &elapsedTime,
                        int &totalSafelyOpenedCell,
                        int gameBoard[][MAX_BOARD_SIZE],
                        int mineBoard[][MAX_BOARD_SIZE]) {
  boardHeight = savedHEIGHT;
  boardWidth = savedWIDTH;
  numMines = savedMINES;
  curMode = savedGameLevel;

  flagLeft = savedFlagLeft;
  elapsedTime = savedTime;
  totalSafelyOpenedCell = savedOpenedCell;

  for (int row = 0; row < savedHEIGHT; row++)
    for (int col = 0; col < savedWIDTH; col++) {
      gameBoard[row][col] = savedGameBoard[row][col];
      mineBoard[row][col] = savedMineBoard[row][col];
    }
}

void transferDataToLeaderboard(long long leaderboard[][MAX_PLAYER_DISPLAY + 1]) {

    for (int mode = 0; mode < NUMBER_OF_GAME_MODE; mode++)
        for (int player = 0; player < MAX_PLAYER_DISPLAY; player++)
            leaderboard[mode][player] = savedLeaderboard[mode][player];
}
