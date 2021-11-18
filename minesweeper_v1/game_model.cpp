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

void constructBoard(GameBoard &gameBoard, const int &currentLevel) {
  gameBoard.currentLevel = currentLevel;
  gameBoard.boardWidth = boardLevelsInfo[currentLevel].width;
  gameBoard.boardHeight = boardLevelsInfo[currentLevel].height;
  gameBoard.numMines = boardLevelsInfo[currentLevel].numMines;
  gameBoard.numFlagsLeft = gameBoard.numMines;
  gameBoard.numOpenedCells = 0;
  gameBoard.elapsedTime = 0;
  gameBoard.boardStatus = "";
  resetBoard(gameBoard);
  generateMineBoard(gameBoard, gameBoard.numMines);
}

void copyBoard(GameBoard &dest, const GameBoard &src) {
  dest.boardWidth = src.boardWidth;
  dest.boardHeight = src.boardHeight;
  dest.numMines = src.numMines;
  dest.numFlagsLeft = src.numFlagsLeft;
  dest.currentLevel = src.currentLevel;
  dest.numOpenedCells = src.numOpenedCells;
  dest.elapsedTime = src.elapsedTime;
  for (int row = 0; row < src.boardHeight; row++) {
    for (int col = 0; col < src.boardWidth; col++) {
      dest.playerBoard[row][col] = src.playerBoard[row][col];
      dest.mineBoard[row][col] = src.mineBoard[row][col];
    }
  }
}

bool revealACell(GameBoard &gameBoard, const int &row, const int &col) {
  if (gameBoard.mineBoard[row][col] == UNKNOWN) {
    uncoverBoard(gameBoard, row, col);
    if (gameBoard.numOpenedCells ==
        gameBoard.boardHeight * gameBoard.boardWidth - gameBoard.numMines) {
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
                            const int &col) {
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
                        neighborsPositions[i].second))
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

void uncoverBoard(GameBoard &gameBoard, const int &row, const int &col) {
  gameBoard.numOpenedCells++;
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
                     neighborsPositions[i].second);
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

const std::string GAMEBOARD_FILE_NAME = "board.txt";
GameBoard savedGameBoard;

void saveBoard(GameBoard &gameBoard) {
  copyBoard(savedGameBoard, gameBoard);
  updateGameBoardDataFile();
}

// GAMEBOARD FILE TEMPLATE
// gamelevel width height mine flag time total_safely_opened_cell
// 0 0 0 0 0 0
//[displayBoard]
//[mineBoard]

void updateGameBoardDataFile() {
  std::ofstream dataFile(GAMEBOARD_FILE_NAME);

  dataFile << savedGameBoard.currentLevel << " " << savedGameBoard.boardWidth
           << " " << savedGameBoard.boardHeight << " "
           << savedGameBoard.numMines << " " << savedGameBoard.numFlagsLeft
           << " " << savedGameBoard.numOpenedCells << " "
           << savedGameBoard.elapsedTime << '\n';
  for (int row = 0; row < savedGameBoard.boardHeight; row++) {
    for (int col = 0; col < savedGameBoard.boardWidth; col++)
      dataFile << savedGameBoard.playerBoard[row][col] << " ";
    dataFile << '\n';
  }
  for (int row = 0; row < savedGameBoard.boardHeight; row++) {
    for (int col = 0; col < savedGameBoard.boardWidth; col++)
      dataFile << savedGameBoard.mineBoard[row][col] << " ";
    dataFile << '\n';
  }

  dataFile.close();
}

bool loadSavedGameBoardDataFile() {
  std::ifstream dataFile(GAMEBOARD_FILE_NAME);
  if (!dataFile) return false;

  dataFile >> savedGameBoard.currentLevel >> savedGameBoard.boardWidth >>
      savedGameBoard.boardHeight >> savedGameBoard.numMines >>
      savedGameBoard.numFlagsLeft >> savedGameBoard.numOpenedCells >>
      savedGameBoard.elapsedTime;
  if (savedGameBoard.boardWidth == 0 || savedGameBoard.boardHeight == 0)
    return false;
  for (int row = 0; row < savedGameBoard.boardHeight; row++)
    for (int col = 0; col < savedGameBoard.boardWidth; col++)
      dataFile >> savedGameBoard.playerBoard[row][col];

  for (int row = 0; row < savedGameBoard.boardHeight; row++)
    for (int col = 0; col < savedGameBoard.boardWidth; col++)
      dataFile >> savedGameBoard.mineBoard[row][col];

  dataFile.close();
  return true;
}

void loadSavedGameBoardData(GameBoard &gameBoard) {
  copyBoard(gameBoard, savedGameBoard);
}
