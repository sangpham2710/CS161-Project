#include "game_model.h"

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include "Leaderboard.h"
#include "global.h"

std::mt19937 randInt(
    std::chrono::steady_clock::now().time_since_epoch().count());

bool revealACell(GameBoard &gameBoard, const int &row, const int &col,
                 int &numOpenedCell, int totalSafeCell) {
  if (gameBoard.mineBoard[row][col] == UNKNOWN) {
    uncoverBoard(gameBoard, row, col, numOpenedCell);
    if (numOpenedCell == totalSafeCell) {
      revealAllMines(gameBoard, true);
      gameBoard.boardStatus = boardStatusOptions[WIN];
      return true;
    }
  } else if (gameBoard.mineBoard[row][col] == MINE) {
    gameBoard.playerBoard[row][col] = MINE;
    revealAllMines(gameBoard, false);
    gameBoard.boardStatus = boardStatusOptions[LOSE];
    return true;
  }

  return false;
}

// ACTION: Reveal Neighbor Cells
bool revealNeighboringCells(GameBoard &gameBoard, const int &row,
                            const int &col, int &numOpenedCell,
                            int totalSafeCell) {
  std::vector<std::pair<int, int>> neighborsPositions = getNeighborsPositions(
      gameBoard.boardWidth, gameBoard.boardHeight, row, col);

  int flagCount =
      countNeighboringCellStates(gameBoard.boardWidth, gameBoard.boardHeight,
                                 gameBoard.playerBoard, row, col, FLAGGED);
  int mineCount =
      countNeighboringCellStates(gameBoard.boardWidth, gameBoard.boardHeight,
                                 gameBoard.mineBoard, row, col, MINE);

  if (flagCount == mineCount) {
    for (int i = 0; i < neighborsPositions.size(); i++)
      if (gameBoard.playerBoard[neighborsPositions[i].first]
                               [neighborsPositions[i].second] == UNKNOWN ||
          gameBoard.playerBoard[neighborsPositions[i].first]
                               [neighborsPositions[i].second] == QUESTIONED) {
        if (revealACell(gameBoard, neighborsPositions[i].first,
                        neighborsPositions[i].second, numOpenedCell,
                        totalSafeCell))
          return true;
      }
  }
  return false;
}

// Clean the board for new play.
void resetBoard(GameBoard &gameBoard) {
  for (int row = 0; row < gameBoard.boardHeight; ++row)
    for (int col = 0; col < gameBoard.boardWidth; ++col)
      gameBoard.playerBoard[row][col] = gameBoard.mineBoard[row][col] = UNKNOWN;
  return;
}

// Check whether the cell [row, column] is in the board or not.
bool isValidCell(const int &width, const int &height, const int &row,
                 const int &col) {
  return (row >= 0) && (row < height) && (col >= 0) && (col < width);
}

// Obviously placing down mines :V
// HAVEN'T CHECKED THE CASE WE HAVE TO GUESS
void generateMineBoard(GameBoard &gameBoard, int mines) {
  int placed = 0;
  while (placed < mines) {
    int row = randInt() % gameBoard.boardHeight;
    int col = randInt() % gameBoard.boardWidth;
    if (gameBoard.mineBoard[row][col] == MINE) continue;  // already a mine
    gameBoard.mineBoard[row][col] = MINE;
    placed++;
  }
}

// First time hit the mine, then move it to somewhere else
void replaceMine(GameBoard &gameBoard, const int &row, const int &col) {
  generateMineBoard(gameBoard, 1);          // add a new mine
  gameBoard.mineBoard[row][col] = UNKNOWN;  // remove the old one
}

// Get the valid neighbor cells
std::vector<std::pair<int, int>> getNeighborsPositions(const int &width,
                                                       const int &height,
                                                       const int &row,
                                                       const int &col) {
  std::vector<std::pair<int, int>> neighborsPositions;

  for (int dx = -1; dx <= 1; dx++)
    for (int dy = -1; dy <= 1; dy++)
      if (dx != 0 || dy != 0)
        if (isValidCell(width, height, row + dx, col + dy))
          neighborsPositions.push_back({row + dx, col + dy});

  return neighborsPositions;
}

int countNeighboringCellStates(const int &width, const int &height,
                               int board[][MAX_BOARD_SIZE], const int &row,
                               const int &col, const int &cellState) {
  std::vector<std::pair<int, int>> neighborsPositions =
      getNeighborsPositions(width, height, row, col);

  int cellCount = 0;
  for (int i = 0; i < neighborsPositions.size(); i++)
    if (board[neighborsPositions[i].first][neighborsPositions[i].second] ==
        cellState)
      cellCount++;

  return cellCount;
}

void uncoverBoard(GameBoard &gameBoard, const int &row, const int &col,
                  int &numOpenedCell) {
  numOpenedCell++;
  int mineCount =
      countNeighboringCellStates(gameBoard.boardWidth, gameBoard.boardHeight,
                                 gameBoard.mineBoard, row, col, MINE);
  gameBoard.playerBoard[row][col] = BLANK + mineCount;

  if (mineCount == 0) {
    std::vector<std::pair<int, int>> neighborsPositions = getNeighborsPositions(
        gameBoard.boardWidth, gameBoard.boardHeight, row, col);

    for (int i = 0; i < neighborsPositions.size(); i++)
      if (gameBoard.playerBoard[neighborsPositions[i].first]
                               [neighborsPositions[i].second] == UNKNOWN)
        uncoverBoard(gameBoard, neighborsPositions[i].first,
                     neighborsPositions[i].second, numOpenedCell);
  }
}

// Show all the mines when the game finished
void revealAllMines(GameBoard &gameBoard, const bool &won) {
  for (int row = 0; row < gameBoard.boardHeight; row++) {
    for (int col = 0; col < gameBoard.boardWidth; col++) {
      if (gameBoard.playerBoard[row][col] == UNKNOWN &&
          gameBoard.mineBoard[row][col] == MINE) {
        if (won)
          gameBoard.playerBoard[row][col] = FLAGGED;
        else
          gameBoard.playerBoard[row][col] = MINE;
      }
    }
  }
}

long long savedElapsedTime, savedLeaderboard[NUM_LEVELS][NUM_RECORDS_PER_LEVEL];
GameBoard savedGameBoard;
int savedNumOpenedCell;

const std::string DATA_FILE_NAME = "data.txt";

void saveLeaderboard(const long long &elapsedTime, const int &gameLevel) {
  if (addToLeaderboard(gameLevel, elapsedTime, savedLeaderboard))
    updateDataFile();
}

void saveBoard(GameBoard &gameBoard, const long long &elapsedTime,
               const int &numOpenedCell) {
  savedGameBoard.boardWidth = gameBoard.boardWidth;
  savedGameBoard.boardHeight = gameBoard.boardHeight;
  savedGameBoard.numMines = gameBoard.numMines;
  savedGameBoard.numFlagsLeft = gameBoard.numFlagsLeft;
  savedGameBoard.currentLevel = gameBoard.currentLevel;
  savedElapsedTime = elapsedTime;
  savedNumOpenedCell = numOpenedCell;

  for (int row = 0; row < gameBoard.boardHeight; row++)
    for (int col = 0; col < gameBoard.boardWidth; col++) {
      savedGameBoard.playerBoard[row][col] = gameBoard.playerBoard[row][col];
      savedGameBoard.mineBoard[row][col] = gameBoard.mineBoard[row][col];
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
  std::ofstream dataFile(DATA_FILE_NAME);
  for (int i = 0; i < NUM_LEVELS; i++) {
    for (int j = 0; j < NUM_RECORDS_PER_LEVEL; j++)
      dataFile << savedLeaderboard[i][j] << " ";
    dataFile << '\n';
  }

  dataFile << savedGameBoard.currentLevel << " " << savedGameBoard.boardWidth
           << " " << savedGameBoard.boardHeight << " "
           << savedGameBoard.numMines << " " << savedGameBoard.numFlagsLeft
           << " " << savedElapsedTime << " " << savedNumOpenedCell;
  for (int row = 0; row < savedGameBoard.boardHeight; row++) {
    dataFile << '\n';
    for (int col = 0; col < savedGameBoard.boardWidth; col++)
      dataFile << savedGameBoard.playerBoard[row][col] << " ";
  }
  for (int row = 0; row < savedGameBoard.boardHeight; row++) {
    dataFile << '\n';
    for (int col = 0; col < savedGameBoard.boardWidth; col++)
      dataFile << savedGameBoard.mineBoard[row][col] << " ";
  }

  dataFile.close();
}

bool loadDataFile() {
  std::ifstream dataFile(DATA_FILE_NAME);
  if (!dataFile) return false;
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 10; j++) dataFile >> savedLeaderboard[i][j];

  dataFile >> savedGameBoard.currentLevel >> savedGameBoard.boardWidth >>
      savedGameBoard.boardWidth >> savedGameBoard.numMines >>
      savedGameBoard.numFlagsLeft >> savedElapsedTime >> savedNumOpenedCell;
  if (savedGameBoard.boardWidth == 0) return false;
  for (int row = 0; row < savedGameBoard.boardWidth; row++)
    for (int col = 0; col < savedGameBoard.boardWidth; col++)
      dataFile >> savedGameBoard.playerBoard[row][col];

  for (int row = 0; row < savedGameBoard.boardWidth; row++)
    for (int col = 0; col < savedGameBoard.boardWidth; col++)
      dataFile >> savedGameBoard.mineBoard[row][col];

  dataFile.close();
  return true;
}

void transferDataToGame(GameBoard &gameBoard, long long &elapsedTime,
                        int &numOpenedCell) {
  gameBoard.boardWidth = savedGameBoard.boardWidth;
  gameBoard.boardHeight = savedGameBoard.boardHeight;
  gameBoard.numMines = savedGameBoard.numMines;
  gameBoard.numFlagsLeft = savedGameBoard.numFlagsLeft;
  gameBoard.currentLevel = savedGameBoard.currentLevel;
  elapsedTime = savedElapsedTime;
  numOpenedCell = savedNumOpenedCell;

  for (int row = 0; row < gameBoard.boardHeight; row++)
    for (int col = 0; col < gameBoard.boardWidth; col++) {
      gameBoard.playerBoard[row][col] = savedGameBoard.playerBoard[row][col];
      gameBoard.mineBoard[row][col] = savedGameBoard.mineBoard[row][col];
    }
}

void transferDataToLeaderboard(
    long long leaderboard[][NUM_RECORDS_PER_LEVEL + 1]) {
  for (int mode = 0; mode < NUM_LEVELS; mode++)
    for (int player = 0; player < NUM_RECORDS_PER_LEVEL; player++)
      leaderboard[mode][player] = savedLeaderboard[mode][player];
}
