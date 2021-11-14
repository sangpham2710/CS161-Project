#include "game_controller.h"

#include <chrono>
#include <iostream>
#include <string>

#include "Leaderboard.h"
#include "NewGame.h"
#include "cmanip.h"
#include "conio.h"
#include "game_model.h"
#include "game_view.h"
#include "global.h"
#include "scene_manager.h"

int startGame(const int &currentLevel) {
  // setConsoleFont(L"Consolas", 600, 20, 40);

  // mineBoard to save the actual values of cells (mine or number).
  // gameBoard to save the uncovered cell, the flagged cells, anything on board
  // displayed to player
  GameBoard gameBoard;
  int cursorRow = 0, cursorCol = 0;
  int totalSafeCell;
  long long savedElapsedTime = 0;
  bool isTimerStarted = false;
  std::chrono::high_resolution_clock::time_point gameStartTime;

  // Game Start
  long long totalElapsedTime = 0;
  int numOpenedCell = 0;
  bool endGame = false;

  if (currentLevel != -1) {  // New game
    gameBoard.currentLevel = currentLevel;
    gameBoard.boardWidth = boardLevelsInfo[currentLevel].width;
    gameBoard.boardHeight = boardLevelsInfo[currentLevel].height;
    gameBoard.numMines = boardLevelsInfo[currentLevel].numMines;
    resetBoard(gameBoard);
    generateMineBoard(gameBoard, gameBoard.numMines);
    gameBoard.numFlagsLeft = gameBoard.numMines;

  } else {  // Continue game
    if (!loadDataFile()) {
      std::cout << "No Previous Play Found!" << '\n'
                << "Press any key to Start a new Game.";
      getUserAction();
      return NewGame();
    }
    transferDataToGame(gameBoard, savedElapsedTime, numOpenedCell);
  }

  totalSafeCell =
      gameBoard.boardHeight * gameBoard.boardWidth - gameBoard.numMines;

  setupDisplay(gameBoard.boardWidth, gameBoard.boardHeight);
  displayBoard(gameBoard.playerBoard, cursorRow, cursorCol, true);
  displayNumFlags(gameBoard.numFlagsLeft, true);
  displayBoardStatus(gameBoard.boardStatus, true);
  // displayTimer(0, true);
  displayTimer(savedElapsedTime / 1000, true);

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
      cursorRow -= isValidCell(gameBoard.boardWidth, gameBoard.boardHeight,
                               cursorRow - 1, cursorCol);
    } else if (action == DOWN) {
      cursorRow += isValidCell(gameBoard.boardWidth, gameBoard.boardHeight,
                               cursorRow + 1, cursorCol);
    } else if (action == LEFT) {
      cursorCol -= isValidCell(gameBoard.boardWidth, gameBoard.boardHeight,
                               cursorRow, cursorCol - 1);
    } else if (action == RIGHT) {
      cursorCol += isValidCell(gameBoard.boardWidth, gameBoard.boardHeight,
                               cursorRow, cursorCol + 1);
    } else if (action == MOUSE1) {
      if (!isTimerStarted) {
        // Start the game timer
        gameStartTime = std::chrono::high_resolution_clock::now() -
                        std::chrono::milliseconds(savedElapsedTime);
        isTimerStarted = true;
      }
      if (numOpenedCell == 0) {
        // Replace first cell if it is a mine
        if (gameBoard.mineBoard[cursorRow][cursorCol] == MINE)
          replaceMine(gameBoard, cursorRow, cursorCol);
      }

      if (gameBoard.playerBoard[cursorRow][cursorCol] == UNKNOWN ||
          gameBoard.playerBoard[cursorRow][cursorCol] == QUESTIONED) {
        endGame = revealACell(gameBoard, cursorRow, cursorCol, numOpenedCell,
                              totalSafeCell);

      } else {
        if (gameBoard.playerBoard[cursorRow][cursorCol] == FLAGGED)
          gameBoard.boardStatus = "Can't reveal a flagged cell.";
        else
          gameBoard.boardStatus = "Can't reveal a revealed cell.";
      }
    } else if (action == MOUSE2) {
      int flagCount = countNeighboringCellStates(
          gameBoard.boardWidth, gameBoard.boardHeight, gameBoard.playerBoard,
          cursorRow, cursorCol, FLAGGED);
      if (gameBoard.playerBoard[cursorRow][cursorCol] == UNKNOWN ||
          gameBoard.playerBoard[cursorRow][cursorCol] == FLAGGED) {
        gameBoard.boardStatus = "Cell must be revealed first!";
      } else if (flagCount !=
                 gameBoard.playerBoard[cursorRow][cursorCol] - BLANK) {
        gameBoard.boardStatus =
            "Please flag the correct number of neighboring mines!";
      } else
        endGame = revealNeighboringCells(gameBoard, cursorRow, cursorCol,
                                         numOpenedCell, totalSafeCell);
    } else if (action == MOUSE3) {
      if (gameBoard.playerBoard[cursorRow][cursorCol] == UNKNOWN) {
        if (gameBoard.numFlagsLeft > 0) {
          gameBoard.numFlagsLeft--;
          gameBoard.playerBoard[cursorRow][cursorCol] = FLAGGED;
        } else {
          gameBoard.boardStatus = "No flag left :((";
        }
      } else if (gameBoard.playerBoard[cursorRow][cursorCol] == FLAGGED) {
        gameBoard.numFlagsLeft++;
        gameBoard.playerBoard[cursorRow][cursorCol] = QUESTIONED;
      } else if (gameBoard.playerBoard[cursorRow][cursorCol] == QUESTIONED) {
        gameBoard.playerBoard[cursorRow][cursorCol] = UNKNOWN;
      } else {
        gameBoard.boardStatus = "Can't flag a revealed cell.";
      }
    } else if (action == SAVE_GAME) {
      saveBoard(gameBoard,
                std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::high_resolution_clock::now() - gameStartTime)
                    .count(),
                numOpenedCell);
      gameBoard.boardStatus = "Game saved!";
    }
    if (endGame) {
      cursorRow = -1;
      cursorCol = -1;
      totalElapsedTime =
          std::chrono::duration_cast<std::chrono::milliseconds>(
              std::chrono::high_resolution_clock::now() - gameStartTime)
              .count();
      if (gameBoard.boardStatus == boardStatusOptions[WIN])
        saveLeaderboard(totalElapsedTime / 1000, gameBoard.currentLevel);
    }
    displayBoard(gameBoard.playerBoard, cursorRow, cursorCol);
    displayNumFlags(gameBoard.numFlagsLeft);
    displayBoardStatus(gameBoard.boardStatus);

    // If game timer has started, display the timer accordingly
    if (gameStartTime != std::chrono::high_resolution_clock::time_point()) {
      displayTimer(
          std::chrono::duration_cast<std::chrono::seconds>(
              std::chrono::high_resolution_clock::now() - gameStartTime)
              .count());
    } else {
      displayTimer(savedElapsedTime / 1000);
    }
  }
  getUserAction();
  return WELCOME;
}
