#include <algorithm>
#include <cctype>
#include <chrono>
#include <functional>
#include <iomanip>
#include <iostream>
#include <random>
#include <vector>

#include "cmanip.h"
#include "conio.h"
#include "windows.h"

// *** CONSTANTS ***

const int WINDOW_WIDTH = 80, WINDOW_HEIGHT = 30;
const int MAX_BOARD_SIZE = 30;
const int CELL_WIDTH = 2, CELL_HEIGHT = 1;
const int BORDER_WIDTH = 2, BORDER_HEIGHT = 1;

const int PANEL_HEIGHT = 3;
const int FPS = 5;
const int CONSOLE_BACKGROUND_COLOR = BRIGHT_WHITE;
const int CONSOLE_TEXT_COLOR = BLACK;
const int CONSOLE_SELECTED_TEXT_COLOR = GREEN;
const int CONSOLE_SELECTED_BACKGROUND_COLOR = BRIGHT_WHITE;
const int BOARD_BORDER_COLOR = BLACK;
// *** END OF CONSTANTS ***

// *** GLOBAL VARIABLES ***
const std::vector<std::string> welcomeOptions = {"New Game", "Leaderboard",
                                                 "Themes", "Exit"};

int selectedWelcomeOption = 0;

int boardWidth, boardHeight;
int numMines;

int PADDING_X, PADDING_Y;

enum cellStates { SELECTED, UNKNOWN, FLAGGED, QUESTIONED, MINE, BLANK };
struct {
  char symbol;
  int backgroundColor;
  int textColor;
} cellStateProps[14] = {
    {' ', LIGHT_AQUA, BLACK},        {'.', GRAY, BLACK},
    {'F', LIGHT_RED, BRIGHT_WHITE},  {'?', YELLOW, BRIGHT_WHITE},
    {'#', RED, BRIGHT_WHITE},        {'.', BRIGHT_WHITE, BLACK},
    {'1', BRIGHT_WHITE, LIGHT_BLUE}, {'2', BRIGHT_WHITE, GREEN},
    {'3', BRIGHT_WHITE, LIGHT_RED},  {'4', BRIGHT_WHITE, BLUE},
    {'5', BRIGHT_WHITE, RED},        {'6', BRIGHT_WHITE, AQUA},
    {'7', BRIGHT_WHITE, BLACK},      {'8', BRIGHT_WHITE, GRAY}};

// *** END OF GLOBAL VARIABLES ***

// *** UTILS ***

std::mt19937 randInt(
    std::chrono::steady_clock::now().time_since_epoch().count());

void resetConsoleScreen() {
  setConsoleColor(CONSOLE_BACKGROUND_COLOR, CONSOLE_TEXT_COLOR);
  clearConsole();
}

int getStartPositionOfACenteredText(const int &textSize) {
  return (WINDOW_WIDTH - textSize) / 2;
}

int printCenteredText(const std::string &text, const int &coordY) {
  int coordX = getStartPositionOfACenteredText(text.size());
  setConsoleCursorPosition(coordX, coordY);
  std::cout << text;
  return coordX;
}

void printColoredTextWrapper(std::function<void()> func,
                             const int &backgroundColor, const int &textColor) {
  setConsoleTextColor(backgroundColor, textColor);
  func();
  setConsoleTextColor(CONSOLE_BACKGROUND_COLOR, CONSOLE_TEXT_COLOR);
}

// *** END OF UTILS ***

void Setup();
void Game();

void startGame();
void displayBoard(int gameBoard[][MAX_BOARD_SIZE], int cursorRow = -1,
                  int cursorCol = -1);
void resetBoard(int gameBoard[][MAX_BOARD_SIZE],
                int mineBoard[][MAX_BOARD_SIZE]);
void generateMineBoard(int mineBoard[][MAX_BOARD_SIZE], int mines);
void replaceMine(int mineBoard[][MAX_BOARD_SIZE], const int &row,
                 const int &col);
bool isValidCell(const int &row, const int &col);
std::vector<std::pair<int, int>> getNeighborsPositions(const int &row,
                                                       const int &col);
int countNeighboringCellStates(int mineBoard[][MAX_BOARD_SIZE], const int &row,
                               const int &col, const int &cellState);
void uncoverBoard(int gameBoard[][MAX_BOARD_SIZE],
                  int mineBoard[][MAX_BOARD_SIZE], const int &row,
                  const int &col, int &nMoves);
void revealAllMines(int gameBoard[][MAX_BOARD_SIZE],
                    int mineBoard[][MAX_BOARD_SIZE], bool won);
bool revealACell(int gameBoard[][MAX_BOARD_SIZE],
                 int mineBoard[][MAX_BOARD_SIZE], const int &row,
                 const int &col, int &totalSafelyOpenedCell, int totalSafeCell,
                 std::string &boardStatus);
bool revealNeighboringCells(int gameBoard[][MAX_BOARD_SIZE],
                            int mineBoard[][MAX_BOARD_SIZE], const int &row,
                            const int &col, int &totalSafelyOpenedCell,
                            int totalSafeCell, std::string &boardStatus);
void waitKeyPressed();

int Welcome();
void displayWelcomeOptions();
int handleWelcomeOptions();

void NewGame();
void ContinueGame();
void ExitGame();
void Leaderboard();
void Themes();

void Setup() {
  setConsoleColor(CONSOLE_BACKGROUND_COLOR, CONSOLE_TEXT_COLOR);
  setWindowName("Minesweeper v1.0");
  setWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
  fixWindowSize();
  showConsoleCursor(0);
}

void displayWelcomeOptions() {
  printCenteredText("WELCOME TO MINESWEEPER V1.0", 0);
  printCenteredText("Press [WASD] to move, [J] to select", 1);

  for (int i = 0; i < welcomeOptions.size(); ++i) {
    setConsoleCursorPosition(
        getStartPositionOfACenteredText(welcomeOptions[i].size()), 3 + i);

    if (i == selectedWelcomeOption) {
      printColoredTextWrapper([&]() { std::cout << welcomeOptions[i]; },
                              CONSOLE_SELECTED_BACKGROUND_COLOR,
                              CONSOLE_SELECTED_TEXT_COLOR);
    } else
      std::cout << welcomeOptions[i];
  }
}

int mod(const int &a, const int &b) { return (a % b + b) % b; }

int handleWelcomeOptions() {
  bool selected = false;
  int currentWelcomeOption = selectedWelcomeOption;
  while (!selected) {
    int action = getUserAction();
    if (action == UP) {
      setConsoleCursorPosition(getStartPositionOfACenteredText(
                                   welcomeOptions[currentWelcomeOption].size()),
                               3 + currentWelcomeOption);

      std::cout << welcomeOptions[currentWelcomeOption];

      currentWelcomeOption =
          mod(currentWelcomeOption - 1, welcomeOptions.size());
      printColoredTextWrapper(
          [&]() {
            setConsoleCursorPosition(
                getStartPositionOfACenteredText(
                    welcomeOptions[currentWelcomeOption].size()),
                3 + currentWelcomeOption);
            std::cout << welcomeOptions[currentWelcomeOption];
          },
          CONSOLE_SELECTED_BACKGROUND_COLOR, CONSOLE_SELECTED_TEXT_COLOR);
    } else if (action == DOWN) {
      setConsoleCursorPosition(getStartPositionOfACenteredText(
                                   welcomeOptions[currentWelcomeOption].size()),
                               3 + currentWelcomeOption);

      std::cout << welcomeOptions[currentWelcomeOption];

      currentWelcomeOption =
          mod(currentWelcomeOption + 1, welcomeOptions.size());

      printColoredTextWrapper(
          [&]() {
            setConsoleCursorPosition(
                getStartPositionOfACenteredText(
                    welcomeOptions[currentWelcomeOption].size()),
                3 + currentWelcomeOption);
            std::cout << welcomeOptions[currentWelcomeOption];
          },
          CONSOLE_SELECTED_BACKGROUND_COLOR, CONSOLE_SELECTED_TEXT_COLOR);
    } else if (action == MOUSE1) {
      selected = 1;
      break;
    }
  }
  return currentWelcomeOption;
}

int Welcome() {
  resetConsoleScreen();
  displayWelcomeOptions();
  return handleWelcomeOptions();
}

void NewGame() {
  resetConsoleScreen();
  std::cout << "Enter width, height, the amound of mines: ";
  std::cin >> boardWidth >> boardHeight >> numMines;
  PADDING_X =
      (WINDOW_WIDTH - (BORDER_WIDTH + CELL_WIDTH * boardWidth + BORDER_WIDTH)) /
      2;
  PADDING_Y =
      (WINDOW_HEIGHT - (PANEL_HEIGHT + BORDER_HEIGHT +
                        CELL_HEIGHT * boardHeight + BORDER_HEIGHT + 2)) /
      2;
  startGame();
}

void ContinueGame() {
  resetConsoleScreen();
  std::cout << "Continue Game\n";
}

void displayExitPrompt() {
  printCenteredText("Do you want to close the program ?", 9);
  printCenteredText("[Y] Yes / [N] No", 10);
}

void handleExit() {
  while (true) {
    int action = getUserAction();
    if (action == YES)
      exit(0);

    else if (action == NO || action == ESCAPE) {
      Game();
      return;
    }
  }
}

void ExitGame() {
  displayExitPrompt();
  handleExit();
}

void Leaderboard() {
  resetConsoleScreen();
  std::cout << "Leaderboard\n";
}

void Themes() {
  resetConsoleScreen();
  std::cout << "Themes\n";
}

void displayBoard(int gameBoard[][MAX_BOARD_SIZE], int cursorRow,
                  int cursorCol) {
  static bool firstCall = true;
  static int oldGameBoard[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
  static int oldCursorRow = 0;
  static int oldCursorCol = 0;
  if (firstCall) {
    memcpy(oldGameBoard, gameBoard,
           MAX_BOARD_SIZE * MAX_BOARD_SIZE * sizeof(int));
    oldCursorRow = cursorRow;
    oldCursorCol = cursorCol;
    firstCall = false;

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
                          << cellStateProps[gameBoard[row][col]].symbol
                          << std::right;
              },
              cellStateProps[SELECTED].backgroundColor,
              cellStateProps[SELECTED].textColor);

        } else {
          printColoredTextWrapper(
              [&]() {
                std::cout << std::setw(CELL_WIDTH) << std::left
                          << cellStateProps[gameBoard[row][col]].symbol
                          << std::right;
              },
              cellStateProps[gameBoard[row][col]].backgroundColor,
              cellStateProps[gameBoard[row][col]].textColor);
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
        if (oldGameBoard[row][col] != gameBoard[row][col]) {
          printColoredTextWrapper(
              [&]() {
                setConsoleCursorPosition(
                    PADDING_X + BORDER_WIDTH + col * CELL_WIDTH,
                    PADDING_Y + PANEL_HEIGHT + BORDER_HEIGHT +
                        row * CELL_HEIGHT);
                std::cout << std::setw(CELL_WIDTH) << std::left
                          << cellStateProps[gameBoard[row][col]].symbol
                          << std::right;
              },
              cellStateProps[gameBoard[row][col]].backgroundColor,
              cellStateProps[gameBoard[row][col]].textColor);
        }
      }
    }

    // Update only if the cursor has changed
    if (oldCursorRow != cursorRow || oldCursorCol != cursorCol ||
        oldGameBoard[cursorRow][cursorCol] != gameBoard[cursorRow][cursorCol]) {
      // Remove old cursor
      if (oldCursorRow != -1 || oldCursorCol != -1) {
        printColoredTextWrapper(
            [&]() {
              setConsoleCursorPosition(
                  PADDING_X + BORDER_WIDTH + oldCursorCol * CELL_WIDTH,
                  PADDING_Y + PANEL_HEIGHT + BORDER_HEIGHT +
                      oldCursorRow * CELL_HEIGHT);
              std::cout << std::setw(CELL_WIDTH) << std::left
                        << cellStateProps[gameBoard[oldCursorRow][oldCursorCol]]
                               .symbol
                        << std::right;
            },
            cellStateProps[gameBoard[oldCursorRow][oldCursorCol]]
                .backgroundColor,
            cellStateProps[gameBoard[oldCursorRow][oldCursorCol]].textColor);
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
                  << cellStateProps[gameBoard[cursorRow][cursorCol]].symbol
                  << std::right;
            },
            cellStateProps[SELECTED].backgroundColor,
            cellStateProps[SELECTED].textColor);
        memcpy(oldGameBoard, gameBoard,
               MAX_BOARD_SIZE * MAX_BOARD_SIZE * sizeof(int));
      }
      // Update old cursor
      oldCursorRow = cursorRow;
      oldCursorCol = cursorCol;
    }
  }
}

void displayNumFlags(const int &numFlags) {
  static bool firstCall = true;
  if (firstCall) {
    setConsoleCursorPosition(PADDING_X, PADDING_Y);
    std::cout << "+-----+---+";
    setConsoleCursorPosition(PADDING_X, PADDING_Y + 1);
    std::cout << "|Flags|" << std::setw(3) << numFlags << "|";
    setConsoleCursorPosition(PADDING_X, PADDING_Y + 2);
    std::cout << "+-----+---+";
    firstCall = false;
  } else {
    // Remove old flag count
    setConsoleCursorPosition(PADDING_X + 7, PADDING_Y + 1);
    std::cout << std::string(3, ' ');
    // Update new flag count
    setConsoleCursorPosition(PADDING_X + 7, PADDING_Y + 1);
    std::cout << std::setw(3) << numFlags;
  }
}

void displayBoardStatus(const std::string &boardStatus) {
  static bool firstCall = true;
  if (firstCall) {
    setConsoleCursorPosition(
        getStartPositionOfACenteredText(boardStatus.size()),
        PADDING_Y + PANEL_HEIGHT + BORDER_HEIGHT + CELL_HEIGHT * boardHeight +
            2);
    std::cout << boardStatus;
    firstCall = false;
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

void displayTimer(const long long &elapsedTime) {
  static bool firstCall = true;
  if (firstCall) {
    setConsoleCursorPosition(
        PADDING_X + BORDER_WIDTH + CELL_WIDTH * boardWidth + BORDER_WIDTH - 10,
        PADDING_Y);
    std::cout << "+----+---+";
    setConsoleCursorPosition(
        PADDING_X + BORDER_WIDTH + CELL_WIDTH * boardWidth + BORDER_WIDTH - 10,
        PADDING_Y + 1);
    std::cout << "|Time|" << std::setw(3) << elapsedTime << "|";
    setConsoleCursorPosition(
        PADDING_X + BORDER_WIDTH + CELL_WIDTH * boardWidth + BORDER_WIDTH - 10,
        PADDING_Y + 2);
    std::cout << "+----+---+";
    firstCall = false;
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
    std::cout << std::setw(3) << elapsedTime;
  }
}

// Game-Logic Here

// Main Process of the game
void startGame() {
  // mineBoard to save the actual values of cells (mine or number).
  // gameBoard to save the uncovered cell, the flagged cells, anything on board
  // displayed to player
  int mineBoard[MAX_BOARD_SIZE][MAX_BOARD_SIZE],
      gameBoard[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
  int cursorRow = 0, cursorCol = 0;
  int totalSafeCell = boardHeight * boardWidth - numMines;
  int numFlagsLeft = numMines;
  std::string boardStatus;
  std::chrono::high_resolution_clock::time_point gameStartTime;

  resetBoard(gameBoard, mineBoard);
  generateMineBoard(mineBoard, numMines);

  // Game Start
  long long totalElapsedTime = 0;
  int totalSafelyOpenedCell = 0;
  bool endGame = false;

  displayBoard(gameBoard, cursorRow, cursorCol);
  displayNumFlags(numFlagsLeft);
  displayBoardStatus(boardStatus);
  displayTimer(0);

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
      if (totalSafelyOpenedCell == 0) {
        // Start the game timer
        gameStartTime = std::chrono::high_resolution_clock::now();

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
      displayTimer(0);
    }
  }
  getUserAction();
}
// ACTION: Reveal a Cell LINKED With Winning and Losing Callout
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
                    int mineBoard[][MAX_BOARD_SIZE], bool won) {
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

void Game() {
  selectedWelcomeOption = Welcome();
  switch (selectedWelcomeOption) {
    case 0:
      NewGame();
      break;
    case 1:
      Leaderboard();
      break;
    case 2:
      Themes();
      break;
    case 3:
      ExitGame();
      break;
  }
}

int main() {
  Setup();
  Game();
  // boardWidth = 9;
  // boardHeight = 9;
  // numMines = 9;
  // int gameboard[30][30];
  // int mineboard[30][30];
  // resetBoard(gameboard, mineboard);
  // displayBoard(gameboard);
  // gameboard[2][2] = FLAGGED;
  // displayBoard(gameboard);
  // getUserAction();
  return 0;
}
