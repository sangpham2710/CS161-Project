#ifndef GLOBAL_H_INCLUDED
#define GLOBAL_H_INCLUDED

#include "cmanip.h"

const int WINDOW_WIDTH = 80, WINDOW_HEIGHT = 30;
const int MAX_BOARD_SIZE = 30;
const int CELL_WIDTH = 2, CELL_HEIGHT = 1;
const int BORDER_WIDTH = 2, BORDER_HEIGHT = 1;

const int PANEL_HEIGHT = 3;
const int FPS = 30;
const int CONSOLE_BACKGROUND_COLOR = BRIGHT_WHITE;
const int CONSOLE_TEXT_COLOR = BLACK;
const int CONSOLE_SELECTED_TEXT_COLOR = GREEN;
const int CONSOLE_SELECTED_BACKGROUND_COLOR = BRIGHT_WHITE;
const int BOARD_BORDER_COLOR = BLACK;

extern int boardWidth, boardHeight;
extern int numMines;

extern int PADDING_X, PADDING_Y;

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

#endif  // GLOBAL_H_INCLUDED
