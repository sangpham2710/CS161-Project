#include <algorithm>
#include <iostream>
#include <vector>
#include <ctype.h>
#include <cstdlib> // for rand() and srand()
#include <ctime> // for time()

#include "cmanip.h"
#include "conio.h"
#include "windows.h"

// *** CONSTANTS ***

const int WINDOW_WIDTH = 50, WINDOW_HEIGHT = 20;
const int MAX_SIZE = 30;

const int CONSOLE_BACKGROUND_COLOR = BRIGHT_WHITE;
const int CONSOLE_TEXT_COLOR = BLACK;
const int CONSOLE_SELECTED_TEXT_COLOR = GREEN;
const int CONSOLE_SELECTED_BACKGROUND_COLOR = BRIGHT_WHITE;

// *** END OF CONSTANTS ***

// *** GLOBAL VARIABLES ***
const std::vector<std::string> welcomeOptions = {"New Game", "Leaderboard",
                                                 "Themes", "Exit"
                                                };

int selectedWelcomeOption = 0;

int MAP_WIDTH, MAP_HEIGHT;
int MINES;
// *** END OF GLOBAL VARIABLES ***

// *** HELPERS ***

void resetConsoleScreen() {
  setConsoleColor(CONSOLE_BACKGROUND_COLOR, CONSOLE_TEXT_COLOR);
  clearConsole();
}

int getStartPositionOfACenteredText(const int& textSize) {
  return (WINDOW_WIDTH - textSize) / 2;
}

int printCenteredText(const std::string& text, const int& coordY) {
  int coordX = getStartPositionOfACenteredText(text.size());
  setConsoleCursorPosition(coordX, coordY);
  std::cout << text;
  return coordX;
}

// *** END OF HELPERS ***

void setup();
void game();

char indexToChar (int index);
int charToIndex(char ch);
void playMinesweeper();
void displayBoard(char gameBoard[][MAX_SIZE]);
void clearBoard(char gameBoard[][MAX_SIZE], char mineBoard[][MAX_SIZE]);
void placeMines(char mineBoard[][MAX_SIZE], int mines);
void replaceMine(int row, int col, char minBoard[][MAX_SIZE]);
bool isValid(int row, int col);
std::vector < std::pair <int, int> > getNeighbors(int row, int col);
int countNeighborMines(int row, int col, char mineBoard[][MAX_SIZE]);
void uncoverBoard(char gameBoard[][MAX_SIZE], char mineBoard[][MAX_SIZE],
                  int row, int col, int *nMoves);
void revealMines(char gameBoard[][MAX_SIZE], char mineBoard[][MAX_SIZE],
                 bool won);
bool revealACell(char gameBoard[][MAX_SIZE], char mineBoard[][MAX_SIZE],
                 int row, int col, int *totalMove, int totalSafeCell);
bool revealNeighborCells(char gameBoard[][MAX_SIZE], char mineBoard[][MAX_SIZE],
                         int row, int col, int *totalMove, int totalSafeCell);
void waitKeyPressed();

int welcome();
void displayWelcomeOptions();
int handleWelcomeOptions();

void newGame();
void continueGame();
void exitGame();
void leaderboard();
void themes();

void setup() {
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

int mod(const int& a, const int& b) {
  return (a % b + b) % b;
}

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

int welcome() {
  resetConsoleScreen();
  displayWelcomeOptions();
  return handleWelcomeOptions();
}

void newGame() {
  resetConsoleScreen();
  std::cout << "Enter width, height, the amound of mines: ";
  std::cin >> MAP_WIDTH >> MAP_HEIGHT >> MINES;
  playMinesweeper();
}

void continueGame() {
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
      game();
      return;
    }
  }
}

void exitGame() {
  displayExitPrompt();
  handleExit();
}

void leaderboard() {
  resetConsoleScreen();
  std::cout << "Leaderboard\n";
}

void themes() {
  resetConsoleScreen();
  std::cout << "Themes\n";
}

// Game-Logic Here

// Functions to switch between char and int for displaying and checking
char indexToChar(int index) {
  if (index < 10)
    return index + '0';
  else
    return 'a' + (index - 10);
}

int charToIndex(char ch) {
  if (ch <= '9')
    return ch - '0';
  else
    return (ch - 'a') + 10;
}

// Main Process of the game
void playMinesweeper() {
  // mineBoard to save the actual values of cells (mine or number).
  // gameBoard to save the uncovered cell, the flagged cells, anything on board displayed to player
  char mineBoard[MAX_SIZE][MAX_SIZE], gameBoard[MAX_SIZE][MAX_SIZE];
  int totalSafeCell = MAP_HEIGHT * MAP_WIDTH - MINES;
  int flagsLeft = MINES;
  clearBoard(gameBoard, mineBoard);
  placeMines(mineBoard, MINES);

  //Game Start
  int totalMove = 0;
  bool endGame = false;

  while (!endGame) {
    displayBoard(gameBoard);
    std::cout << flagsLeft << " flag(s) left" << '\n' << '\n';

    // Get Input
    int row, col;
    char action;

    do {
      std::cout << "Enter your move, (row, column) -> ";
      std::cin >> row >> col;
    } while (!isValid(row, col));

    do {
      std::cout << "Enter your action, " << '\n' <<
                "safe(s)/flag(f)/revealNeighbor(r) -> ";
      std::cin >> action;
      action = tolower(action);
    } while (action != 's' && action != 'f' && action != 'r');

    // Check first move is a mine or not. If yes then move the mine away.
    if (totalMove == 0)
      if (mineBoard[row][col] == '#')
        replaceMine(row, col, mineBoard);

    // ACTION: Reveal a cell
    if (action == 's') {
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
        char ch;
        ch = _getch();
      }
    }

    // ACTION: Flag a Cell
    if (action == 'f') {
      if (gameBoard[row][col] == '.') {
        if (flagsLeft != 0) {
          flagsLeft--;
          gameBoard[row][col] = 'F';
        } else {
          std::cout << '\n' << "No flag left :((  ";

          std::cout << '\n' << "Press any key to continue. ";
          char ch;
          ch = _getch();
        }
      } else if (gameBoard[row][col] == 'F') {
        flagsLeft++;
        gameBoard[row][col] = '.';
      } else {
        std::cout << '\n' << "Illegal move. Cell is revealed. ";

        std::cout << '\n' << "Press any key to continue. ";
        char ch;
        ch = _getch();
      }
    }

    // ACTION: Reveal Neighbor Cell
    if (action == 'r') {
      if (gameBoard[row][col] == '.') {
        std::cout << '\n' << "Cell must be revealed first!  ";

        std::cout << '\n' << "Press any key to continue. ";
        char ch;
        ch = _getch();
      } else if (gameBoard[row][col] == 'F') {
        std::cout << '\n' << "Cell is flagged. Unflag to reveal. ";

        std::cout << '\n' << "Press any key to continue. ";
        char ch;
        ch = _getch();
      } else
        endGame = revealNeighborCells(gameBoard, mineBoard, row, col, &totalMove,
                                      totalSafeCell);
    }
  }
}

// ACTION: Reveal a Cell LINKED With Winning and Losing Callout
bool revealACell(char gameBoard[][MAX_SIZE], char mineBoard[][MAX_SIZE],
                 int row, int col, int *totalMove, int totalSafeCell) {
  if (mineBoard[row][col] == '.') {
    uncoverBoard(gameBoard, mineBoard, row, col, totalMove);
    if ((*totalMove) == totalSafeCell) {
      revealMines(gameBoard, mineBoard, true);
      displayBoard(gameBoard);
      std::cout << '\n' << "Congratulation! You won!" << '\n';
      return true;
    }
  } else if (mineBoard[row][col] == '#') {
    gameBoard[row][col] = '#';
    displayBoard(gameBoard);
    revealMines(gameBoard, mineBoard, false);
    displayBoard(gameBoard);
    std::cout << '\n' << "Umm... Quite a big explosion, right?" << '\n';
    return true;
  }

  return false;
}

// ACTION: Reveal Neighbor Cells
bool revealNeighborCells(char gameBoard[][MAX_SIZE], char mineBoard[][MAX_SIZE],
                         int row, int col, int *totalMove, int totalSafeCell) {
  std::vector < std::pair <int, int> > neighbor = getNeighbors(row, col);

  int countFlag = 0;
  int countMine = countNeighborMines(row, col, mineBoard);

  for (int i = 0; i < neighbor.size(); i++) {
    if (gameBoard[neighbor[i].first][neighbor[i].second] == 'F')
      countFlag++;
  }

  std::cout << countFlag << " " << countMine << '\n';

  if (countFlag == countMine) {
    for (int i = 0; i < neighbor.size(); i++)
      if (gameBoard[neighbor[i].first][neighbor[i].second] == '.') {
        if (revealACell(gameBoard, mineBoard, neighbor[i].first, neighbor[i].second,
                        totalMove, totalSafeCell))
          return true;
      }
  }
  return false;
}

// Clean the board for new play.
void clearBoard(char gameBoard[][MAX_SIZE], char mineBoard[][MAX_SIZE]) {
  for (int row = 0; row < MAP_HEIGHT; ++row)
    for (int col = 0; col < MAP_WIDTH; ++col)
      gameBoard[row][col] = mineBoard[row][col] = '.';
  return;
}

// Check whether the cell [row, column] is in the board or not.
bool isValid(int row, int col) {
  return (row >= 0) && (row < MAP_HEIGHT) && (col >= 0) && (col < MAP_WIDTH);
}

// Obviously placing down mines :V
// HAVEN'T CHECKED THE CASE WE HAVE TO GUESS
void placeMines(char mineBoard[][MAX_SIZE], int mines) {
  int placed = 0;
  while (placed < mines) {
    int row = rand() % MAP_HEIGHT;
    int col = rand() % MAP_WIDTH;
    if (mineBoard[row][col] == '#') continue; // already a mine
    mineBoard[row][col] = '#';
    placed++;
  }
}

// First time hit the mine, then move it to somewhere else
void replaceMine(int row, int col, char mineBoard[][MAX_SIZE]) {
  placeMines(mineBoard, 1);  // add a new mine
  mineBoard[row][col] = '.'; // remove the old one
  return;
}

void displayBoard(char gameBoard[][MAX_SIZE]) {
  resetConsoleScreen();

  //Number on first line to help player locate the cell
  std::cout << "    ";
  for (int i = 0; i < MAP_WIDTH; i++)
    std::cout << i << ' ';
  std::cout << '\n' << '\n';

  // The next rows
  for (int row = 0; row < MAP_HEIGHT; ++row) {
    std::cout << row << "   ";
    for (int col = 0; col < MAP_WIDTH; ++col)
      std::cout << gameBoard[row][col] << " ";
    std::cout << "  " << row << '\n';
  }

  // Bottom number line
  std::cout << '\n' << "    ";
  for (int i = 0; i < MAP_WIDTH; i++)
    std::cout << i << ' ';
  std::cout << '\n';
}

// NOT WORKING -> INFINITE LOOP
// SOLUTION: Temporarily using directly
void waitKeyPressed() {
  std::cout << '\n' << "Press any key to continue. ";
  char ch;
  ch = _getch();
}

// Get the valid neighbor cells
std::vector < std::pair <int, int> > getNeighbors(int row, int col) {
  std::vector < std::pair <int, int> > neighbors;

  for (int dx = -1; dx <= 1; dx++)
    for (int dy = -1; dy <= 1; dy++)
      if (dx != 0 || dy != 0)
        if (isValid(row + dx, col + dy))
          neighbors.push_back(std::make_pair(row + dx, col + dy));

  return neighbors;
}

int countNeighborMines(int row, int col, char mineBoard[][MAX_SIZE]) {
  std::vector < std::pair <int, int> > neighbors = getNeighbors(row, col);

  int Count = 0;
  for (int i = 0; i < neighbors.size(); i++)
    if (mineBoard[neighbors[i].first][neighbors[i].second] == '#')
      Count++;

  return Count;
}

void uncoverBoard(char gameBoard[][MAX_SIZE], char mineBoard[][MAX_SIZE],
                  int row, int col, int *totalMove) {
  (*totalMove)++;
  int Count = countNeighborMines(row, col, mineBoard);
  gameBoard[row][col] = indexToChar(Count);

  if (Count == 0) {
    std::vector < std::pair <int, int> > neighbors = getNeighbors(row, col);

    for (int i = 0; i < neighbors.size(); i++)
      if (gameBoard[neighbors[i].first][neighbors[i].second] == '.')
        uncoverBoard(gameBoard, mineBoard, neighbors[i].first, neighbors[i].second,
                     totalMove);
  }
}

// Show all the mines when the game finished
void revealMines(char gameBoard[][MAX_SIZE], char mineBoard[][MAX_SIZE],
                 bool won) {
  for (int row = 0; row < MAP_HEIGHT; row++) {
    for (int col = 0; col < MAP_WIDTH; col++) {
      if (gameBoard[row][col] == '.' && mineBoard[row][col] == '#') {
        if (won)
          gameBoard[row][col] = 'F';
        else
          gameBoard[row][col] = '#';

        //displayBoard(gameBoard);
      }
    }
  }
}

void game() {
  selectedWelcomeOption = welcome();
  switch (selectedWelcomeOption) {
  case 0:
    newGame();
    break;
  case 1:
    leaderboard();
    break;
  case 2:
    themes();
    break;
  case 3:
    exitGame();
    break;
  }
}

int main() {
  srand(time(0));
  setup();
  game();
  return 0;
}
