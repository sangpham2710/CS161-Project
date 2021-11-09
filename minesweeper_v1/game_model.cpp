#include "game_model.h"

#include <algorithm>
#include <chrono>
#include <random>
#include <vector>

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
      boardStatus = "Congratulation! You won!";
      return true;
    }
  } else if (mineBoard[row][col] == MINE) {
    gameBoard[row][col] = MINE;
    revealAllMines(gameBoard, mineBoard, false);
    boardStatus = "Umm... Quite a big explosion, right?";
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