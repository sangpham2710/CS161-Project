#ifndef GAMELEVEL_H_INCLUDED
#define GAMELEVEL_H_INCLUDED

#include <string>
#include <vector>
#include "main_utils.h"

const struct {
    int WIDTH;
    int HEIGHT;
    int MINES;
} boardInfo[] = {{9, 9, 10}, {16, 16, 40}, {30, 16, 99}};
const std::vector<std::string> GameLevelOptions = {"Beginner | " + intToString(boardInfo[0].MINES) + " mines | " + intToString(boardInfo[0].WIDTH) + "x" + intToString(boardInfo[0].HEIGHT) + " Grid",
                                                   "Beginner | " + intToString(boardInfo[1].MINES) + " mines | " + intToString(boardInfo[1].WIDTH) + "x" + intToString(boardInfo[1].HEIGHT) + " Grid",
                                                   "Beginner | " + intToString(boardInfo[2].MINES) + " mines | " + intToString(boardInfo[2].WIDTH) + "x" + intToString(boardInfo[2].HEIGHT) + " Grid"};
int GameLevel();
void displayGameLevelOptions(const int& oldGameLevelOption);
int handleGameLevelOptions(const int& oldGameLevelOption);

#endif  // GAMELEVEL_H_INCLUDED
