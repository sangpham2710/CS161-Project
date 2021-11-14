#ifndef GLOBAL_H_INCLUDED
#define GLOBAL_H_INCLUDED

#include <string>
#include <vector>

#include "cmanip.h"
#include "main_utils.h"

const int WINDOW_WIDTH = 75, WINDOW_HEIGHT = 25;  // 1200x800
// const int WINDOW_WIDTH = 60, WINDOW_HEIGHT = 20; // 1200x800
const int MAX_BOARD_SIZE = 40;
const int CELL_WIDTH = 2, CELL_HEIGHT = 1;
const int BORDER_WIDTH = 2, BORDER_HEIGHT = 1;

const int PANEL_HEIGHT = 3;
const int FPS = 30;
const int CONSOLE_BACKGROUND_COLOR = BRIGHT_WHITE;
const int CONSOLE_TEXT_COLOR = BLACK;
const int CONSOLE_SELECTED_TEXT_COLOR = GREEN;
const int CONSOLE_SELECTED_BACKGROUND_COLOR = BRIGHT_WHITE;
const int BOARD_BORDER_COLOR = BLACK;

// Game board
struct GameBoard {
  int boardWidth;
  int boardHeight;
  int numMines;
  int numFlagsLeft;
  int currentLevel;
  std::string boardStatus;
  int playerBoard[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
  int mineBoard[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
};

// Cell states
enum cellStates { SELECTED, UNKNOWN, FLAGGED, QUESTIONED, MINE, BLANK };
const struct {
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

// Levels
enum Levels { BEGINNER, INTERMEDIATE, EXPERT, NUM_LEVELS };
const struct {
  std::string name;
  int width;
  int height;
  int numMines;
} boardLevelsInfo[3] = {{"Beginner", 9, 9, 10},
                        {"Intermediate", 16, 16, 40},
                        {"Expert", 30, 16, 99}};

const std::vector<std::string> levelOptions = {
    boardLevelsInfo[BEGINNER].name + " | " +
        intToString(boardLevelsInfo[BEGINNER].numMines) + " mines | " +
        intToString(boardLevelsInfo[BEGINNER].width) + "x" +
        intToString(boardLevelsInfo[BEGINNER].height) + " grid",
    boardLevelsInfo[INTERMEDIATE].name + " | " +
        intToString(boardLevelsInfo[INTERMEDIATE].numMines) + " mines | " +
        intToString(boardLevelsInfo[INTERMEDIATE].width) + "x" +
        intToString(boardLevelsInfo[INTERMEDIATE].height) + " grid",
    boardLevelsInfo[EXPERT].name + " | " +
        intToString(boardLevelsInfo[EXPERT].numMines) + " mines | " +
        intToString(boardLevelsInfo[EXPERT].width) + "x" +
        intToString(boardLevelsInfo[EXPERT].height) + " grid"};

// Leaderboard
const int NUM_PLAYERS_PER_LEVEL = 10;

#endif  // GLOBAL_H_INCLUDED
