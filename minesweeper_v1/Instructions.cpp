#include "instructions.h"

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

#include "cmanip.h"
#include "game_model.h"
#include "global.h"
#include "main_utils.h"
#include "scene_manager.h"
#include "windows.h"

const long long MAX_TIME = (long long)1e18;

int PADDING_INSTRUCTIONS_X, PADDING_INSTRUCTIONS_Y;

// 3 che do
// 0 0 0 0 0 0 0 0 0 0
// 0 0 0 0 0 0 0 0 0 0
// 0 0 0 0 0 0 0 0 0 0
const std::vector<std::string> instructions = {
    R"([WASD]/[Arrow keys])",
    "Move Cursor",
    "[J]/[Enter]",
    "Select | Open A Cell",
    "[K]",
    "Open Neighboring Cells",
    "[L]",
    "Flag A Cell",
    "[O]",
    "Save Game",
    "[R]",
    "Replay Game",
    "[Y]",
    "Yes",
    "[N]",
    "No",
    R"([Escape])",
    R"(Quit Game | Back to Menu)",
};

int instructionsWidth =
    strlen(R"(           [Escape]: Quit Game | Back to Menu)");
int instructionsHeight = instructions.size() / 2;

std::vector<std::string> getInstructionHeaderText() {
  return {
      R"( _  _   ___ __      __  _____  ___    ___  _       _ __   __)",
      R"(| || | / _ \\ \    / / |_   _|/ _ \  | _ \| |     /_\\ \ / /)",
      R"(| __ || (_) |\ \/\/ /    | | | (_) | |  _/| |__  / _ \\ V / )",
      R"(|_||_| \___/  \_/\_/     |_|  \___/  |_|  |____|/_/ \_\|_|  )",
  };
}

int getInstructionsPosition() { return PADDING_INSTRUCTIONS_Y + 1; }

void displayInstructionsHeaderAndFooter() {
  std::vector<std::string> headerText = getInstructionHeaderText();
  const int spacing = 1;

  for (int i = 0; i < headerText.size(); i++)
    printCenteredText(headerText[i], 3 + i);

  printCenteredText(R"([J] Back to Menu)", getWindowHeight() - 2);
}

int Instructions() {
  setupInstructionsDisplay();
  displayInstructions();
  while (true) {
    int action = getUserAction();
    if (action == MOUSE1 || action == ESCAPE) return WELCOME;
  }
}

void displayInstructions() {
  resetConsoleScreen();

  int cellWidth = instructions[0].size();

  displayInstructionsHeaderAndFooter();

  for (int i = 0; i < instructions.size(); i++) {
    setConsoleCursorPosition(
        PADDING_INSTRUCTIONS_X + (cellWidth - instructions[i].size()) / 2 - 5,
        PADDING_INSTRUCTIONS_Y + i / 2 + 2);
    std::cout << instructions[i];

    setConsoleCursorPosition(PADDING_INSTRUCTIONS_X + cellWidth - 2,
                             PADDING_INSTRUCTIONS_Y + i / 2 + 2);
    std::cout << "   :      " << instructions[i + 1];
    // printCenteredText(std::string((cellWidth - instruction[i].size()) / 2, '
    // ') + instruction[i] + std::string((cellWidth - instruction[i].size()) /
    // 2, ' ') + std::string(instructionWidth - cellWidth, ' '),
    // PADDING_INSTRUCTION_Y + i / 2 + 2);
    // printCenteredText(std::string(instruction[i].size(), ' ') + instruction[i
    // + 1] + std::string(instructionWidth - instruction[i].size() -
    // instruction[i + 1].size(), ' '), PADDING_INSTRUCTION_Y + i / 2 + 2);
    i++;
  }
}

void setupInstructionsDisplay() {
  PADDING_INSTRUCTIONS_X = (getWindowWidth() - instructionsWidth) / 2;
  PADDING_INSTRUCTIONS_Y = (getWindowHeight() - instructionsHeight) / 2;
}
