#include <algorithm>
#include <iostream>
#include <vector>

#include "cmanip.h"
#include "conio.h"
#include "windows.h"

// *** CONSTANTS ***

const int WINDOW_WIDTH = 50, WINDOW_HEIGHT = 20;
const int MAP_WIDTH = 20, MAP_HEIGHT = 20;

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
  for (int row = 0; row < MAP_WIDTH; ++row) {
    for (int column = 0; column < MAP_HEIGHT; ++column) std::cout << "@";
    std::cout << '\n';
  }
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
  setup();
  game();
  return 0;
}
