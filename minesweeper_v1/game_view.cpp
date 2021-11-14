#include "game_view.h"

#include <iomanip>
#include <iostream>

#include "main_utils.h"

int PADDING_X, PADDING_Y;
int boardWidth, boardHeight;

void setupDisplay(const int &_boardWidth, const int &_boardHeight) {
  boardWidth = _boardWidth;
  boardHeight = _boardHeight;
  PADDING_X =
      (WINDOW_WIDTH - (BORDER_WIDTH + CELL_WIDTH * boardWidth + BORDER_WIDTH)) /
      2;
  PADDING_Y =
      (WINDOW_HEIGHT - (PANEL_HEIGHT + BORDER_HEIGHT +
                        CELL_HEIGHT * boardHeight + BORDER_HEIGHT + 2)) /
      2;
}

void displayBoard(int playerBoard[][MAX_BOARD_SIZE], const int &cursorRow,
                  const int &cursorCol, const bool &firstCall) {
  static int oldPlayerBoard[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
  static int oldCursorRow = 0;
  static int oldCursorCol = 0;
  if (firstCall) {
    memcpy(oldPlayerBoard, playerBoard,
           MAX_BOARD_SIZE * MAX_BOARD_SIZE * sizeof(int));
    oldCursorRow = cursorRow;
    oldCursorCol = cursorCol;

    resetConsoleScreen();

    std::string borderX =
        std::string(BORDER_WIDTH + boardWidth * CELL_WIDTH + BORDER_WIDTH, ' ');
    // Top border
    printColoredTextWrapper(
        [&]() {
          setConsoleCursorPosition(PADDING_X, PADDING_Y + PANEL_HEIGHT);
          std::cout << borderX;
        },
        BOARD_BORDER_COLOR, BOARD_BORDER_COLOR);

    std::cout << '\n';

    for (int row = 0; row < boardHeight; ++row) {
      // Left border
      setConsoleCursorPosition(
          PADDING_X,
          PADDING_Y + PANEL_HEIGHT + BORDER_HEIGHT + row * CELL_HEIGHT);
      printColoredTextWrapper(
          []() { std::cout << std::string(BORDER_WIDTH, ' '); },
          BOARD_BORDER_COLOR, BOARD_BORDER_COLOR);
      for (int col = 0; col < boardWidth; ++col) {
        if (row == cursorRow && col == cursorCol) {
          printColoredTextWrapper(
              [&]() {
                std::cout << std::setw(CELL_WIDTH) << std::left
                          << cellStateProps[playerBoard[row][col]].symbol
                          << std::right;
              },
              cellStateProps[SELECTED].backgroundColor,
              cellStateProps[SELECTED].textColor);

        } else {
          printColoredTextWrapper(
              [&]() {
                std::cout << std::setw(CELL_WIDTH) << std::left
                          << cellStateProps[playerBoard[row][col]].symbol
                          << std::right;
              },
              cellStateProps[playerBoard[row][col]].backgroundColor,
              cellStateProps[playerBoard[row][col]].textColor);
        }
      }
      // Right border
      printColoredTextWrapper(
          []() { std::cout << std::string(BORDER_WIDTH, ' '); },
          BOARD_BORDER_COLOR, BOARD_BORDER_COLOR);
    }
    // Bottom border
    printColoredTextWrapper(
        [&]() {
          setConsoleCursorPosition(PADDING_X, PADDING_Y + PANEL_HEIGHT +
                                                  BORDER_HEIGHT +
                                                  CELL_HEIGHT * boardHeight);
          std::cout << borderX;
        },
        BOARD_BORDER_COLOR, BOARD_BORDER_COLOR);
    std::cout << '\n';
  } else {
    // Update gameboard
    for (int row = 0; row < boardHeight; ++row) {
      for (int col = 0; col < boardWidth; ++col) {
        // Update only if the cell has changed
        if (oldPlayerBoard[row][col] != playerBoard[row][col]) {
          printColoredTextWrapper(
              [&]() {
                setConsoleCursorPosition(
                    PADDING_X + BORDER_WIDTH + col * CELL_WIDTH,
                    PADDING_Y + PANEL_HEIGHT + BORDER_HEIGHT +
                        row * CELL_HEIGHT);
                std::cout << std::setw(CELL_WIDTH) << std::left
                          << cellStateProps[playerBoard[row][col]].symbol
                          << std::right;
              },
              cellStateProps[playerBoard[row][col]].backgroundColor,
              cellStateProps[playerBoard[row][col]].textColor);
        }
      }
    }

    // Update only if the cursor has changed
    if (oldCursorRow != cursorRow || oldCursorCol != cursorCol ||
        oldPlayerBoard[cursorRow][cursorCol] !=
            playerBoard[cursorRow][cursorCol]) {
      // Remove old cursor
      if (oldCursorRow != -1 || oldCursorCol != -1) {
        printColoredTextWrapper(
            [&]() {
              setConsoleCursorPosition(
                  PADDING_X + BORDER_WIDTH + oldCursorCol * CELL_WIDTH,
                  PADDING_Y + PANEL_HEIGHT + BORDER_HEIGHT +
                      oldCursorRow * CELL_HEIGHT);
              std::cout
                  << std::setw(CELL_WIDTH) << std::left
                  << cellStateProps[playerBoard[oldCursorRow][oldCursorCol]]
                         .symbol
                  << std::right;
            },
            cellStateProps[playerBoard[oldCursorRow][oldCursorCol]]
                .backgroundColor,
            cellStateProps[playerBoard[oldCursorRow][oldCursorCol]].textColor);
      }
      // Update new cursor
      if (cursorRow != -1 || cursorCol != -1) {
        printColoredTextWrapper(
            [&]() {
              setConsoleCursorPosition(
                  PADDING_X + BORDER_WIDTH + cursorCol * CELL_WIDTH,
                  PADDING_Y + PANEL_HEIGHT + BORDER_HEIGHT +
                      cursorRow * CELL_HEIGHT);
              std::cout
                  << std::setw(CELL_WIDTH) << std::left
                  << cellStateProps[playerBoard[cursorRow][cursorCol]].symbol
                  << std::right;
            },
            cellStateProps[SELECTED].backgroundColor,
            cellStateProps[SELECTED].textColor);
        memcpy(oldPlayerBoard, playerBoard,
               MAX_BOARD_SIZE * MAX_BOARD_SIZE * sizeof(int));
      }
      // Update old cursor
      oldCursorRow = cursorRow;
      oldCursorCol = cursorCol;
    }
  }
}

void displayNumFlags(const int &numFlags, const bool &firstCall) {
  if (firstCall) {
    setConsoleCursorPosition(PADDING_X, PADDING_Y);
    std::cout << "+-----+---+";
    setConsoleCursorPosition(PADDING_X, PADDING_Y + 1);
    std::cout << "|Flags|" << std::setw(3) << numFlags << "|";
    setConsoleCursorPosition(PADDING_X, PADDING_Y + 2);
    std::cout << "+-----+---+";
  } else {
    // Remove old flag count
    setConsoleCursorPosition(PADDING_X + 7, PADDING_Y + 1);
    std::cout << std::string(3, ' ');
    // Update new flag count
    setConsoleCursorPosition(PADDING_X + 7, PADDING_Y + 1);
    std::cout << std::setw(3) << numFlags;
  }
}

void displayBoardStatus(const std::string &boardStatus, const bool &firstCall) {
  if (firstCall) {
    setConsoleCursorPosition(
        getStartPositionOfACenteredText(boardStatus.size()),
        PADDING_Y + PANEL_HEIGHT + BORDER_HEIGHT + CELL_HEIGHT * boardHeight +
            2);
    std::cout << boardStatus;
  } else {
    // Remove old board status
    setConsoleCursorPosition(0, PADDING_Y + PANEL_HEIGHT + BORDER_HEIGHT +
                                    CELL_HEIGHT * boardHeight + 2);
    std::cout << std::string(WINDOW_WIDTH, ' ');
    // Update new board status
    setConsoleCursorPosition(
        getStartPositionOfACenteredText(boardStatus.size()),
        PADDING_Y + PANEL_HEIGHT + BORDER_HEIGHT + CELL_HEIGHT * boardHeight +
            2);
    std::cout << boardStatus;
  }
}

void displayTimer(const long long &elapsedTime, const bool &firstCall) {
  if (firstCall) {
    setConsoleCursorPosition(
        PADDING_X + BORDER_WIDTH + CELL_WIDTH * boardWidth + BORDER_WIDTH - 10,
        PADDING_Y);
    std::cout << "+----+---+";
    setConsoleCursorPosition(
        PADDING_X + BORDER_WIDTH + CELL_WIDTH * boardWidth + BORDER_WIDTH - 10,
        PADDING_Y + 1);
    std::cout << "|Time|" << std::setw(3) << elapsedTime / 1000 << "|";
    setConsoleCursorPosition(
        PADDING_X + BORDER_WIDTH + CELL_WIDTH * boardWidth + BORDER_WIDTH - 10,
        PADDING_Y + 2);
    std::cout << "+----+---+";
  } else {
    // Remove old timer
    setConsoleCursorPosition(
        PADDING_X + BORDER_WIDTH + CELL_WIDTH * boardWidth + BORDER_WIDTH - 4,
        PADDING_Y + 1);
    std::cout << std::string(3, ' ');
    // Update new timer
    setConsoleCursorPosition(
        PADDING_X + BORDER_WIDTH + CELL_WIDTH * boardWidth + BORDER_WIDTH - 4,
        PADDING_Y + 1);
    std::cout << std::setw(3) << elapsedTime / 1000;
  }
}
