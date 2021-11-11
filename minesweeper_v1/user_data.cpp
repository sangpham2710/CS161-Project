#include <fstream>
#include <iostream>
#include <string>

#include "global.h"
#include "user_data.h"

long long savedTime, savedLeaderboard[3][10];
int savedGameLevel, savedWIDTH, savedHEIGHT, savedMINES, savedFlagLeft, savedOpenedCell,
    savedGameBoard[MAX_BOARD_SIZE][MAX_BOARD_SIZE], savedMineBoard[MAX_BOARD_SIZE][MAX_BOARD_SIZE];

const std::string FILENAME = "data.txt";

void addToLeaderboard(const long long &elapsedTime, const int &gameLevel){

}

void saveBoard(const int &WIDTH, const int &HEIGHT, const int &MINES, const int &flagLeft,
               const long long &elapsedTime, const int &totalSafelyOpenedCell,
               int gameBoard[][MAX_BOARD_SIZE], int mineBoard[][MAX_BOARD_SIZE]) {
    savedWIDTH = WIDTH;
    savedHEIGHT = HEIGHT;
    savedMINES = MINES;
    savedFlagLeft = flagLeft;
    savedTime = elapsedTime;
    savedOpenedCell = totalSafelyOpenedCell;

    for (int row = 0; row < HEIGHT; row++)
    for (int col = 0; col < WIDTH; col++) {
        savedGameBoard[row][col] = gameBoard[row][col];
        savedMineBoard[row][col] = mineBoard[row][col];
    }
    updateDataFile();
}

//DATA FILE TEMPLATE
//3 che do
//0 0 0 0 0 0 0 0 0 0
//0 0 0 0 0 0 0 0 0 0
//0 0 0 0 0 0 0 0 0 0
//width height mine time total_safely_opened_cell
//0 0 0 0 0
//[displayBoard]
//[mineBoard]
void updateDataFile() {
    std::ofstream dataFile(FILENAME);
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 10; j++)
            dataFile << savedLeaderboard[i][j] << " ";
        dataFile << '\n';
    }

    dataFile << savedWIDTH << " " << savedHEIGHT << " " << savedMINES << " " << savedTime << " " << savedOpenedCell;
    for (int row = 0; row < savedHEIGHT; row++){
        dataFile << '\n';
        for (int col = 0; col < savedWIDTH; col++)
            dataFile << savedGameBoard[row][col] << " ";
    }
    for (int row = 0; row < savedHEIGHT; row++){
        dataFile << '\n';
        for (int col = 0; col < savedWIDTH; col++)
            dataFile << savedMineBoard[row][col] << " ";
    }

    dataFile.close();
}

void loadDataFile() {
    std::ifstream dataFile(FILENAME);
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 10; j++)
            dataFile >> savedLeaderboard[i][j];

    dataFile >> savedWIDTH >> savedHEIGHT >> savedMINES >> savedTime >> savedOpenedCell;
    for (int row = 0; row < savedHEIGHT; row++)
        for (int col = 0; col < savedWIDTH; col++)
            dataFile >> savedGameBoard[row][col];

    for (int row = 0; row < savedHEIGHT; row++)
        for (int col = 0; col < savedWIDTH; col++)
            dataFile >> savedMineBoard[row][col];

    dataFile.close();
}

void transferDataToGame(int &flagLeft, long long &elapsedTime, int &totalSafelyOpenedCell,
                        int gameBoard[][MAX_BOARD_SIZE], int mineBoard[][MAX_BOARD_SIZE]) {
    boardHeight = savedHEIGHT;
    boardWidth = savedWIDTH;
    numMines = savedMINES;

    flagLeft = savedFlagLeft;
    elapsedTime = savedTime;
    totalSafelyOpenedCell = savedOpenedCell;

    for (int row = 0; row < savedHEIGHT; row++)
    for (int col = 0; col < savedWIDTH; col++) {
        gameBoard[row][col] = savedGameBoard[row][col];
        mineBoard[row][col] = savedMineBoard[row][col];
    }
}
