#include "game_controller.h"

#include <chrono>
#include <iostream>
#include <string>

#include "cmanip.h"
#include "conio.h"
#include "game_data.h"
#include "game_model.h"
#include "game_view.h"
#include "global.h"
#include "scene_manager.h"

int numMines, boardWidth, boardHeight;
int PADDING_X, PADDING_Y;

int startGame(const int &state) {
  // setConsoleFont(L"Consolas", 600, 20, 40);

  // mineBoard to save the actual values of cells (mine or number).
  // gameBoard to save the uncovered cell, the flagged cells, anything on board
  // displayed to player
  int mineBoard[MAX_BOARD_SIZE][MAX_BOARD_SIZE],
      gameBoard[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
  int cursorRow = 0, cursorCol = 0;
  int totalSafeCell;
  int numFlagsLeft;
  long long savedTime = 0;
  bool isTimerStarted = false;
  std::string boardStatus;
  std::chrono::high_resolution_clock::time_point gameStartTime;

  // Game Start
  long long totalElapsedTime = 0;
  int totalSafelyOpenedCell = 0;
  bool endGame = false;

  if (state == NEW) {
    resetBoard(gameBoard, mineBoard);
    generateMineBoard(mineBoard, numMines);

    totalSafeCell = boardHeight * boardWidth - numMines;
    numFlagsLeft = numMines;

  } else if (state == CONTINUE) {
    loadDataFile();
    transferDataToGame(numFlagsLeft, savedTime, totalSafelyOpenedCell,
                       gameBoard, mineBoard);

    PADDING_X = (WINDOW_WIDTH -
                 (BORDER_WIDTH + CELL_WIDTH * boardWidth + BORDER_WIDTH)) /
                2;
    PADDING_Y =
        (WINDOW_HEIGHT - (PANEL_HEIGHT + BORDER_HEIGHT +
                          CELL_HEIGHT * boardHeight + BORDER_HEIGHT + 2)) /
        2;
  }

  displayBoard(gameBoard, cursorRow, cursorCol, true);
  displayNumFlags(numFlagsLeft, true);
  displayBoardStatus(boardStatus, true);
  // displayTimer(0, true);
  displayTimer(savedTime / 1000, true);
  getUserAction();

  while (!endGame) {
    // Get input from player
    int action = NO_ACTION;

    std::chrono::high_resolution_clock::time_point updateInputStartTime =
        std::chrono::high_resolution_clock::now();

    while (std::chrono::duration_cast<std::chrono::milliseconds>(
               std::chrono::high_resolution_clock::now() - updateInputStartTime)
               .count() <= 1000 / FPS) {
      if (kbhit()) {
        action = getUserAction();
        break;
      }
    }

    if (action == ESCAPE) {
      endGame = true;
      break;
    } else if (action == UP) {
      cursorRow -= isValidCell(cursorRow - 1, cursorCol);
    } else if (action == DOWN) {
      cursorRow += isValidCell(cursorRow + 1, cursorCol);
    } else if (action == LEFT) {
      cursorCol -= isValidCell(cursorRow, cursorCol - 1);
    } else if (action == RIGHT) {
      cursorCol += isValidCell(cursorRow, cursorCol + 1);
    } else if (action == MOUSE1) {
      if (!isTimerStarted) {
        // Start the game timer
        gameStartTime = std::chrono::high_resolution_clock::now() -
                        std::chrono::milliseconds(savedTime);
        isTimerStarted = true;
      }
      if (totalSafelyOpenedCell == 0) {
        // Replace first cell if it is a mine
        if (mineBoard[cursorRow][cursorCol] == MINE)
          replaceMine(mineBoard, cursorRow, cursorCol);
      }

      if (gameBoard[cursorRow][cursorCol] == UNKNOWN ||
          gameBoard[cursorRow][cursorCol] == QUESTIONED) {
        endGame =
            revealACell(gameBoard, mineBoard, cursorRow, cursorCol,
                        totalSafelyOpenedCell, totalSafeCell, boardStatus);

      } else {
        if (gameBoard[cursorRow][cursorCol] == FLAGGED)
          boardStatus = "Can't reveal a flagged cell.";
        else
          boardStatus = "Can't reveal a revealed cell.";
      }
    } else if (action == MOUSE2) {
      int flagCount =
          countNeighboringCellStates(gameBoard, cursorRow, cursorCol, FLAGGED);
      if (gameBoard[cursorRow][cursorCol] == UNKNOWN ||
          gameBoard[cursorRow][cursorCol] == FLAGGED) {
        boardStatus = "Cell must be revealed first!";
      } else if (flagCount != gameBoard[cursorRow][cursorCol] - BLANK) {
        boardStatus = "Please flag the correct number of neighboring mines!";
      } else
        endGame = revealNeighboringCells(gameBoard, mineBoard, cursorRow,
                                         cursorCol, totalSafelyOpenedCell,
                                         totalSafeCell, boardStatus);
    } else if (action == MOUSE3) {
      if (gameBoard[cursorRow][cursorCol] == UNKNOWN) {
        if (numFlagsLeft > 0) {
          numFlagsLeft--;
          gameBoard[cursorRow][cursorCol] = FLAGGED;
        } else {
          boardStatus = "No flag left :((";
        }
      } else if (gameBoard[cursorRow][cursorCol] == FLAGGED) {
        numFlagsLeft++;
        gameBoard[cursorRow][cursorCol] = QUESTIONED;
      } else if (gameBoard[cursorRow][cursorCol] == QUESTIONED) {
        gameBoard[cursorRow][cursorCol] = UNKNOWN;
      } else {
        boardStatus = "Can't flag a revealed cell.";
      }
    } else if (action == SAVE_GAME) {
      saveBoard(boardWidth, boardHeight, numMines, numFlagsLeft,
                std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::high_resolution_clock::now() - gameStartTime)
                    .count(),
                totalSafelyOpenedCell, gameBoard, mineBoard);
      boardStatus = "Game saved!";
    }
    if (endGame) {
      cursorRow = -1;
      cursorCol = -1;
      totalElapsedTime =
          std::chrono::duration_cast<std::chrono::milliseconds>(
              std::chrono::high_resolution_clock::now() - gameStartTime)
              .count();
    }
    displayBoard(gameBoard, cursorRow, cursorCol);
    displayNumFlags(numFlagsLeft);
    displayBoardStatus(boardStatus);

    // If game timer has started, display the timer accordingly
    if (gameStartTime != std::chrono::high_resolution_clock::time_point()) {
      displayTimer(
          std::chrono::duration_cast<std::chrono::seconds>(
              std::chrono::high_resolution_clock::now() - gameStartTime)
              .count());
    } else {
      displayTimer(savedTime / 1000);
    }
  }
  getUserAction();
  return WELCOME;
}
