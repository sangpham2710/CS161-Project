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

const int WINDOW_WIDTH = 50, WINDOW_HEIGHT = 20;
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

enum cellStates { UNKNOWN, FLAGGED, QUESTIONED, MINE };
struct {
  char symbol;
  int backgroundColor;
  int textColor;
} cellStateProps[4] = {{'.', BRIGHT_WHITE, BLACK},
                       {'F', BRIGHT_WHITE, YELLOW},
                       {'?', BRIGHT_WHITE, GREEN},
                       {'#', BRIGHT_WHITE, RED}};

// *** END OF GLOBAL VARIABLES ***

// *** HELPERS ***

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

// *** END OF HELPERS ***

void Setup();
void Game();

char intToChar(int index);
int charToIndex(char ch);
void startGame();
void displayBoard(char gameBoard[][MAX_BOARD_SIZE]);
void resetBoard(char gameBoard[][MAX_BOARD_SIZE],
                char mineBoard[][MAX_BOARD_SIZE]);
void generateMineBoard(char mineBoard[][MAX_BOARD_SIZE], int mines);
void replaceMine(const int &row, const int &col,
                 char minBoard[][MAX_BOARD_SIZE]);
bool isValidCell(const int &row, int col);
std::vector<std::pair<int, int> > getNeighborsPositions(const int &row,
                                                        const int &col);
int countNeighboringMines(const int &row, const int &col,
                          char mineBoard[][MAX_BOARD_SIZE]);
void uncoverBoard(char gameBoard[][MAX_BOARD_SIZE],
                  char mineBoard[][MAX_BOARD_SIZE], const int &row,
                  const int &col, int *nMoves);
void revealAllMines(char gameBoard[][MAX_BOARD_SIZE],
                    char mineBoard[][MAX_BOARD_SIZE], bool won);
bool revealACell(char gameBoard[][MAX_BOARD_SIZE],
                 char mineBoard[][MAX_BOARD_SIZE], const int &row,
                 const int &col, int *totalMove, int totalSafeCell);
bool revealNeighboringCells(char gameBoard[][MAX_BOARD_SIZE],
                            char mineBoard[][MAX_BOARD_SIZE], const int &row,
                            const int &col, int *totalMove, int totalSafeCell);
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

// Functions to switch between char and int for displaying and checking
char intToChar(int val) { return val + '0'; }

// int charToIndex(char ch) {
//   if (ch <= '9')
//     return ch - '0';
//   else
//     return (ch - 'a') + 10;
// }

// Main Process of the game
void startGame() {
  // mineBoard to save the actual values of cells (mine or number).
  // gameBoard to save the uncovered cell, the flagged cells, anything on board
  // displayed to player
  char mineBoard[MAX_BOARD_SIZE][MAX_BOARD_SIZE],
      gameBoard[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
  int totalSafeCell = boardHeight * boardWidth - numMines;
  int numFlagsLeft = numMines;
  resetBoard(gameBoard, mineBoard);
  generateMineBoard(mineBoard, numMines);

  // Game Start
  int totalMove = 0;
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
      std::cout << "Enter your action, " << '\n'
                << "safe(s)/flag(f)/revealNeighbor(r) -> ";
      action = getUserAction();
    } while (action != MOUSE1 && action != MOUSE2 && action != MOUSE3);
    // Check first move is a mine or not. If yes then move the mine away.
    if (totalMove == 0)
      if (mineBoard[row][col] == '#') replaceMine(row, col, mineBoard);

    // ACTION: Reveal a cell
    if (action == MOUSE1) {
      if (gameBoard[row][col] == '.')
        endGame = revealACell(gameBoard, mineBoard, row, col, &totalMove,
                              totalSafeCell);

      else {
        std::cout << "Illegal move. ";
        if (mineBoard[row][col] == 'f')
          std::cout << '\n' << "Cell is flagged. Unflag to reveal. ";
        else
          std::cout << '\n' << "Cell is revealed. ";

        std::cout << '\n' << "Press any key to continue. ";
        getUserAction();
      }
    }

    // ACTION: Flag a Cell
    if (action == MOUSE2) {
      if (gameBoard[row][col] == '.') {
        if (numFlagsLeft != 0) {
          numFlagsLeft--;
          gameBoard[row][col] = 'F';
        } else {
          std::cout << '\n' << "No flag left :((  ";

          std::cout << '\n' << "Press any key to continue. ";
          getUserAction();
        }
      } else if (gameBoard[row][col] == 'F') {
        numFlagsLeft++;
        gameBoard[row][col] = '.';
      } else {
        std::cout << '\n' << "Illegal move. Cell is revealed. ";

        std::cout << '\n' << "Press any key to continue. ";
        getUserAction();
      }
    }

    // ACTION: Reveal Neighbor Cell
    if (action == MOUSE3) {
      if (gameBoard[row][col] == '.') {
        std::cout << '\n' << "Cell must be revealed first!  ";

        std::cout << '\n' << "Press any key to continue. ";
        getUserAction();
      } else if (gameBoard[row][col] == 'F') {
        std::cout << '\n' << "Cell is flagged. Unflag to reveal. ";

        std::cout << '\n' << "Press any key to continue. ";
        getUserAction();
      } else
        endGame = revealNeighboringCells(gameBoard, mineBoard, row, col,
                                         &totalMove, totalSafeCell);
    }
  }
}

// ACTION: Reveal a Cell LINKED With Winning and Losing Callout
bool revealACell(char gameBoard[][MAX_BOARD_SIZE],
                 char mineBoard[][MAX_BOARD_SIZE], const int &row,
                 const int &col, int *totalMove, int totalSafeCell) {
  if (mineBoard[row][col] == '.') {
    uncoverBoard(gameBoard, mineBoard, row, col, totalMove);
    if ((*totalMove) == totalSafeCell) {
      revealAllMines(gameBoard, mineBoard, true);
      displayBoard(gameBoard);
      std::cout << '\n' << "Congratulation! You won!" << '\n';
      return true;
    }
  } else if (mineBoard[row][col] == '#') {
    gameBoard[row][col] = '#';
    displayBoard(gameBoard);
    revealAllMines(gameBoard, mineBoard, false);
    displayBoard(gameBoard);
    std::cout << '\n' << "Umm... Quite a big explosion, right?" << '\n';
    return true;
  }

  return false;
}

// ACTION: Reveal Neighbor Cells
bool revealNeighboringCells(char gameBoard[][MAX_BOARD_SIZE],
                            char mineBoard[][MAX_BOARD_SIZE], const int &row,
                            const int &col, int *totalMove, int totalSafeCell) {
  std::vector<std::pair<int, int> > neighbor = getNeighborsPositions(row, col);

  int countFlag = 0;
  int countMine = countNeighboringMines(row, col, mineBoard);

  for (int i = 0; i < neighbor.size(); i++) {
    if (gameBoard[neighbor[i].first][neighbor[i].second] == 'F') countFlag++;
  }

  std::cout << countFlag << " " << countMine << '\n';

  if (countFlag == countMine) {
    for (int i = 0; i < neighbor.size(); i++)
      if (gameBoard[neighbor[i].first][neighbor[i].second] == '.') {
        if (revealACell(gameBoard, mineBoard, neighbor[i].first,
                        neighbor[i].second, totalMove, totalSafeCell))
          return true;
      }
  }
  return false;
}

// Clean the board for new play.
void resetBoard(char gameBoard[][MAX_BOARD_SIZE],
                char mineBoard[][MAX_BOARD_SIZE]) {
  for (int row = 0; row < boardHeight; ++row)
    for (int col = 0; col < boardWidth; ++col)
      gameBoard[row][col] = mineBoard[row][col] = '.';
  return;
}

// Check whether the cell [row, column] is in the board or not.
bool isValidCell(const int &row, int col) {
  return (row >= 0) && (row < boardHeight) && (col >= 0) && (col < boardWidth);
}

// Obviously placing down mines :V
// HAVEN'T CHECKED THE CASE WE HAVE TO GUESS
void generateMineBoard(char mineBoard[][MAX_BOARD_SIZE], int mines) {
  int placed = 0;
  while (placed < mines) {
    int row = randInt() % boardHeight;
    int col = randInt() % boardWidth;
    if (mineBoard[row][col] == '#') continue;  // already a mine
    mineBoard[row][col] = '#';
    placed++;
  }
}

// First time hit the mine, then move it to somewhere else
void replaceMine(const int &row, const int &col,
                 char mineBoard[][MAX_BOARD_SIZE]) {
  generateMineBoard(mineBoard, 1);  // add a new mine
  mineBoard[row][col] = '.';        // remove the old one
  return;
}

void displayBoard(char gameBoard[][MAX_BOARD_SIZE]) {
  resetConsoleScreen();

  // Number on first line to help player locate the cell
  std::cout << "    ";
  for (int i = 0; i < boardWidth; i++) std::cout << i << ' ';
  std::cout << '\n' << '\n';

  // The next rows
  for (int row = 0; row < boardHeight; ++row) {
    std::cout << row << "   ";
    for (int col = 0; col < boardWidth; ++col)
      std::cout << gameBoard[row][col] << " ";
    std::cout << "  " << row << '\n';
  }

  // Bottom number line
  std::cout << '\n' << "    ";
  for (int i = 0; i < boardWidth; i++) std::cout << i << ' ';
  std::cout << '\n';
}

// NOT WORKING -> INFINITE LOOP
// SOLUTION: Temporarily using directly
// void waitKeyPressed() {
//   std::cout << '\n' << "Press any key to continue. ";
//   char ch;
//   ch = _getch();
// }

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
                          char mineBoard[][MAX_BOARD_SIZE]) {
  std::vector<std::pair<int, int> > neighborsPositions =
      getNeighborsPositions(row, col);

  int cellCount = 0;
  for (int i = 0; i < neighborsPositions.size(); i++)
    if (mineBoard[neighborsPositions[i].first][neighborsPositions[i].second] ==
        '#')
      cellCount++;

  return cellCount;
}

void uncoverBoard(char gameBoard[][MAX_BOARD_SIZE],
                  char mineBoard[][MAX_BOARD_SIZE], const int &row,
                  const int &col, int *totalMove) {
  (*totalMove)++;
  int mineCount = countNeighboringMines(row, col, mineBoard);
  gameBoard[row][col] = intToChar(mineCount);

  if (mineCount == 0) {
    std::vector<std::pair<int, int> > neighborsPositions =
        getNeighborsPositions(row, col);

    for (int i = 0; i < neighborsPositions.size(); i++)
      if (gameBoard[neighborsPositions[i].first]
                   [neighborsPositions[i].second] == '.')
        uncoverBoard(gameBoard, mineBoard, neighborsPositions[i].first,
                     neighborsPositions[i].second, totalMove);
  }
}

// Show all the mines when the game finished
void revealAllMines(char gameBoard[][MAX_BOARD_SIZE],
                    char mineBoard[][MAX_BOARD_SIZE], bool won) {
  for (int row = 0; row < boardHeight; row++) {
    for (int col = 0; col < boardWidth; col++) {
      if (gameBoard[row][col] == '.' && mineBoard[row][col] == '#') {
        if (won)
          gameBoard[row][col] = 'F';
        else
          gameBoard[row][col] = '#';

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
