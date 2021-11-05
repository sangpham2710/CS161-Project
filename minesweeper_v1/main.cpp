#include <algorithm>
#include <cctype>
#include <chrono>
#include <iostream>
#include <random>
#include <vector>

#include "cmanip.h"
#include "conio.h"
#include "windows.h"

// *** CONSTANTS ***

const int WINDOW_WIDTH = 80, WINDOW_HEIGHT = 30;
const int MAX_BOARD_SIZE = 30;

const int CONSOLE_BACKGROUND_COLOR = BRIGHT_WHITE;
const int CONSOLE_TEXT_COLOR = BLACK;
const int CONSOLE_SELECTED_TEXT_COLOR = GREEN;
const int CONSOLE_SELECTED_BACKGROUND_COLOR = BRIGHT_WHITE;

// *** END OF CONSTANTS ***

// *** GLOBAL VARIABLES ***
const std::vector<std::string> welcomeOptions = {"New Game", "Leaderboard",
                                                 "Themes", "Exit"};

int selectedWelcomeOption = 0;

int boardWidth, boardHeight;
int numMines;

enum cellStates { UNKNOWN, FLAGGED, QUESTIONED, MINE, BLANK };
struct {
  char symbol;
  int backgroundColor;
  int textColor;
} cellStateProps[5] = {{'.', GRAY, BLACK},
                       {'F', LIGHT_RED, BRIGHT_WHITE},
                       {'?', YELLOW, BRIGHT_WHITE},
                       {'#', RED, BRIGHT_WHITE},
                       {' ', BRIGHT_WHITE, BLACK}};

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

// *** END OF UTILS ***

void Setup();
void Game();

void startGame();
void displayBoard(int gameBoard[][MAX_BOARD_SIZE]);
void resetBoard(int gameBoard[][MAX_BOARD_SIZE],
                int mineBoard[][MAX_BOARD_SIZE]);
void generateMineBoard(int mineBoard[][MAX_BOARD_SIZE], int mines);
void replaceMine(const int &row, const int &col,
                 int mineBoard[][MAX_BOARD_SIZE]);
bool isValidCell(const int &row, int col);
std::vector<std::pair<int, int> > getNeighborsPositions(const int &row,
                                                        const int &col);
int countNeighboringMines(const int &row, const int &col,
                          int mineBoard[][MAX_BOARD_SIZE]);
void uncoverBoard(int gameBoard[][MAX_BOARD_SIZE],
                  int mineBoard[][MAX_BOARD_SIZE], const int &row,
                  const int &col, int *nMoves);
void revealAllMines(int gameBoard[][MAX_BOARD_SIZE],
                    int mineBoard[][MAX_BOARD_SIZE], bool won);
bool revealACell(int gameBoard[][MAX_BOARD_SIZE],
                 int mineBoard[][MAX_BOARD_SIZE], const int &row,
                 const int &col, int *totalSafelyOpenedCell, int totalSafeCell);
bool revealNeighboringCells(int gameBoard[][MAX_BOARD_SIZE],
                            int mineBoard[][MAX_BOARD_SIZE], const int &row,
                            const int &col, int *totalSafelyOpenedCell,
                            int totalSafeCell);
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
      setConsoleTextColor(CONSOLE_SELECTED_BACKGROUND_COLOR,
                          CONSOLE_SELECTED_TEXT_COLOR);

      std::cout << welcomeOptions[i];

      setConsoleTextColor(CONSOLE_BACKGROUND_COLOR, CONSOLE_TEXT_COLOR);

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

      setConsoleCursorPosition(getStartPositionOfACenteredText(
                                   welcomeOptions[currentWelcomeOption].size()),
                               3 + currentWelcomeOption);

      setConsoleTextColor(CONSOLE_SELECTED_BACKGROUND_COLOR,
                          CONSOLE_SELECTED_TEXT_COLOR);
      std::cout << welcomeOptions[currentWelcomeOption];
      setConsoleTextColor(CONSOLE_BACKGROUND_COLOR, CONSOLE_TEXT_COLOR);

    } else if (action == DOWN) {
      setConsoleCursorPosition(getStartPositionOfACenteredText(
                                   welcomeOptions[currentWelcomeOption].size()),
                               3 + currentWelcomeOption);

      std::cout << welcomeOptions[currentWelcomeOption];

      currentWelcomeOption =
          mod(currentWelcomeOption + 1, welcomeOptions.size());

      setConsoleCursorPosition(getStartPositionOfACenteredText(
                                   welcomeOptions[currentWelcomeOption].size()),
                               3 + currentWelcomeOption);

      setConsoleTextColor(CONSOLE_SELECTED_BACKGROUND_COLOR,
                          CONSOLE_SELECTED_TEXT_COLOR);
      std::cout << welcomeOptions[currentWelcomeOption];
      setConsoleTextColor(CONSOLE_BACKGROUND_COLOR, CONSOLE_TEXT_COLOR);

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

// Game-Logic Here

// Main Process of the game
void startGame() {
  // mineBoard to save the actual values of cells (mine or number).
  // gameBoard to save the uncovered cell, the flagged cells, anything on board
  // displayed to player
  int mineBoard[MAX_BOARD_SIZE][MAX_BOARD_SIZE],
      gameBoard[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
  int totalSafeCell = boardHeight * boardWidth - numMines;
  int numFlagsLeft = numMines;
  resetBoard(gameBoard, mineBoard);
  generateMineBoard(mineBoard, numMines);

  // Game Start
  int totalSafelyOpenedCell = 0;
  bool endGame = false;

  while (!endGame) {
    displayBoard(gameBoard);
    std::cout << numFlagsLeft << " flag(s) left" << '\n' << '\n';

    // Get Input
    int row, col;
    int action;

    do {
      std::cout << "Enter your move, (row, column) -> ";
      std::cin >> row >> col;
    } while (!isValidCell(row, col));

    do {
      std::cout << "Enter your action: " << '\n'
                << "j: reveal; " << '\n'
                << "k: flag (first time) / question (already flagged); " << '\n'
                << "l: reveal all neighboring cells. " << '\n'
                << "-> ";
      action = getUserAction();
    } while (action != MOUSE1 && action != MOUSE2 && action != MOUSE3);
    // Check first move is a mine or not. If yes then move the mine away.
    if (totalSafelyOpenedCell == 0)
      if (mineBoard[row][col] == MINE) replaceMine(row, col, mineBoard);

    // ACTION: Reveal a cell
    if (action == MOUSE1) {
      if (gameBoard[row][col] == UNKNOWN)
        endGame = revealACell(gameBoard, mineBoard, row, col,
                              &totalSafelyOpenedCell, totalSafeCell);

      else {
        std::cout << "Illegal move. ";
        if (mineBoard[row][col] == FLAGGED)
          std::cout << '\n' << "Cell is flagged. Unflag to reveal. ";
        else
          std::cout << '\n' << "Cell is revealed. ";

        waitKeyPressed();
      }
    }

    // ACTION: Flag and Question a Cell
    if (action == MOUSE2) {
      if (gameBoard[row][col] == UNKNOWN) {
        if (numFlagsLeft != 0) {
          numFlagsLeft--;
          gameBoard[row][col] = FLAGGED;
        } else {
          std::cout << '\n' << "No flag left :((  ";

          waitKeyPressed();
        }
      } else if (gameBoard[row][col] == FLAGGED) {
        numFlagsLeft++;
        gameBoard[row][col] = QUESTIONED;
      } else {
        gameBoard[row][col] = UNKNOWN;
      }
    }

    // ACTION: Reveal Neighbor Cell
    if (action == MOUSE3) {
      if (gameBoard[row][col] == UNKNOWN || gameBoard[row][col] == FLAGGED) {
        std::cout << '\n' << "Cell must be revealed first!  ";

        waitKeyPressed();
      } else
        endGame = revealNeighboringCells(gameBoard, mineBoard, row, col,
                                         &totalSafelyOpenedCell, totalSafeCell);
    }
  }
}

// ACTION: Reveal a Cell LINKED With Winning and Losing Callout
bool revealACell(int gameBoard[][MAX_BOARD_SIZE],
                 int mineBoard[][MAX_BOARD_SIZE], const int &row,
                 const int &col, int *totalSafelyOpenedCell,
                 int totalSafeCell) {
  if (mineBoard[row][col] == UNKNOWN) {
    uncoverBoard(gameBoard, mineBoard, row, col, totalSafelyOpenedCell);
    if ((*totalSafelyOpenedCell) == totalSafeCell) {
      revealAllMines(gameBoard, mineBoard, true);
      displayBoard(gameBoard);
      std::cout << '\n' << "Congratulation! You won!" << '\n';
      return true;
    }
  } else if (mineBoard[row][col] == MINE) {
    gameBoard[row][col] = MINE;
    displayBoard(gameBoard);
    revealAllMines(gameBoard, mineBoard, false);
    displayBoard(gameBoard);
    std::cout << '\n' << "Umm... Quite a big explosion, right?" << '\n';
    return true;
  }

  return false;
}

// ACTION: Reveal Neighbor Cells
bool revealNeighboringCells(int gameBoard[][MAX_BOARD_SIZE],
                            int mineBoard[][MAX_BOARD_SIZE], const int &row,
                            const int &col, int *totalSafelyOpenedCell,
                            int totalSafeCell) {
  std::vector<std::pair<int, int> > neighbor = getNeighborsPositions(row, col);

  int flagCount = 0;
  int mineCount = countNeighboringMines(row, col, mineBoard);

  for (int i = 0; i < neighbor.size(); i++) {
    if (gameBoard[neighbor[i].first][neighbor[i].second] == FLAGGED)
      flagCount++;
  }

  if (flagCount == mineCount) {
    for (int i = 0; i < neighbor.size(); i++)
      if (gameBoard[neighbor[i].first][neighbor[i].second] == UNKNOWN) {
        if (revealACell(gameBoard, mineBoard, neighbor[i].first,
                        neighbor[i].second, totalSafelyOpenedCell,
                        totalSafeCell))
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
bool isValidCell(const int &row, int col) {
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
void replaceMine(const int &row, const int &col,
                 int mineBoard[][MAX_BOARD_SIZE]) {
  generateMineBoard(mineBoard, 1);  // add a new mine
  mineBoard[row][col] = UNKNOWN;    // remove the old one
  return;
}

void displayBoard(int gameBoard[][MAX_BOARD_SIZE]) {
  resetConsoleScreen();

  // Number on first line to help player locate the cell
  std::cout << "    ";
  for (int i = 0; i < boardWidth; i++) std::cout << i << ' ';
  std::cout << '\n' << '\n';

  // The next rows
  for (int row = 0; row < boardHeight; ++row) {
    std::cout << row << "   ";
    for (int col = 0; col < boardWidth; ++col) {
      if (gameBoard[row][col] <= BLANK) {
        setConsoleTextColor(cellStateProps[gameBoard[row][col]].backgroundColor,
                            cellStateProps[gameBoard[row][col]].textColor);

        std::cout << cellStateProps[gameBoard[row][col]].symbol << " ";

        setConsoleTextColor(CONSOLE_BACKGROUND_COLOR, CONSOLE_TEXT_COLOR);
      } else
        std::cout << gameBoard[row][col] - BLANK << " ";
    }
    std::cout << "  " << row << '\n';
  }

  // Bottom number line
  std::cout << '\n' << "    ";
  for (int i = 0; i < boardWidth; i++) std::cout << i << ' ';
  std::cout << '\n';
}

// New Fixed using getUserAction();
void waitKeyPressed() {
  std::cout << '\n' << "Press any key to continue. ";
  getUserAction();
}

// Get the valid neighbor cells
std::vector<std::pair<int, int> > getNeighborsPositions(const int &row,
                                                        const int &col) {
  std::vector<std::pair<int, int> > neighborsPositions;

  for (int dx = -1; dx <= 1; dx++)
    for (int dy = -1; dy <= 1; dy++)
      if (dx != 0 || dy != 0)
        if (isValidCell(row + dx, col + dy))
          neighborsPositions.push_back({row + dx, col + dy});

  return neighborsPositions;
}

int countNeighboringMines(const int &row, const int &col,
                          int mineBoard[][MAX_BOARD_SIZE]) {
  std::vector<std::pair<int, int> > neighborsPositions =
      getNeighborsPositions(row, col);

  int cellCount = 0;
  for (int i = 0; i < neighborsPositions.size(); i++)
    if (mineBoard[neighborsPositions[i].first][neighborsPositions[i].second] ==
        MINE)
      cellCount++;

  return cellCount;
}

void uncoverBoard(int gameBoard[][MAX_BOARD_SIZE],
                  int mineBoard[][MAX_BOARD_SIZE], const int &row,
                  const int &col, int *totalSafelyOpenedCell) {
  (*totalSafelyOpenedCell)++;
  int mineCount = countNeighboringMines(row, col, mineBoard);
  gameBoard[row][col] = BLANK + mineCount;

  if (mineCount == 0) {
    std::vector<std::pair<int, int> > neighborsPositions =
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

        // displayBoard(gameBoard);
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
  return 0;
}
