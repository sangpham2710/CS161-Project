#include "Instruction.h"

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

int PADDING_INSTRUCTION_X, PADDING_INSTRUCTION_Y;

// 3 che do
// 0 0 0 0 0 0 0 0 0 0
// 0 0 0 0 0 0 0 0 0 0
// 0 0 0 0 0 0 0 0 0 0
const std::vector <std::string> instruction = {
    R"([WASD]/[Arrow keys])", "Move Cursor",
    "[J]/[Enter]", "Select | Open A Cell",
    "[K]", "Open Neighboring Cells",
    "[L]", "Flag A Cell",
    "[O]", "Save Game",
    "[R]", "Replay Game",
    "[Y]", "Yes",
    "[N]", "No",
    R"([Escape])", R"(Quit Game | Back to Menu)"
};

int instructionWidth =
              strlen(R"(           [Escape]: Quit Game | Back to Menu)"),
          instructionHeight = instruction.size() / 2;
//{
//    R"()",
//    R"()",
//    R"()",
//    R"()",
//  };

std::vector<std::string> getInstructionHeaderText() {
  return {
    R"( _  _   ___ __      __  _____  ___    ___  _       _ __   __)",
    R"(| || | / _ \\ \    / / |_   _|/ _ \  | _ \| |     /_\\ \ / /)",
    R"(| __ || (_) |\ \/\/ /    | | | (_) | |  _/| |__  / _ \\ V / )",
    R"(|_||_| \___/  \_/\_/     |_|  \___/  |_|  |____|/_/ \_\|_|  )",
  };
}

int getInstructionPosition() {
    return PADDING_INSTRUCTION_Y + 1;
}

void displayInstructionHeaderAndFooter() {
  std::vector<std::string> headerText = getInstructionHeaderText();
  const int spacing = 1;

  for (int i = 0; i < headerText.size(); i++)
    printCenteredText(headerText[i], 3 + i);

  //printCenteredText("Version 1.0", PADDING_INSTRUCTION_Y - spacing);
  printCenteredText(R"([J] Back to Menu)",
                    getWindowHeight() - 1 - 1);
}

int Instruction() {
  setupInstructionDisplay();
  displayInstruction();
  while (true) {
    int action = getUserAction();
    if (action == MOUSE1 || action == ESCAPE)
      return WELCOME;
  }
}

void displayInstruction() {
  resetConsoleScreen();

  int cellWidth = instruction[0].size();

  displayInstructionHeaderAndFooter();

  //etConsoleCursorPosition(PADDING_INSTRUCTION_X, getInstructionPosition());

  for (int i = 0; i < instruction.size(); i ++) {
    setConsoleCursorPosition(PADDING_INSTRUCTION_X + (cellWidth - instruction[i].size()) / 2 - 5, PADDING_INSTRUCTION_Y + i / 2 + 2);
    std::cout << instruction[i];

    setConsoleCursorPosition(PADDING_INSTRUCTION_X + cellWidth - 2, PADDING_INSTRUCTION_Y + i / 2 + 2);
    std::cout << "   :      " << instruction[i + 1];
    //printCenteredText(std::string((cellWidth - instruction[i].size()) / 2, ' ') + instruction[i] + std::string((cellWidth - instruction[i].size()) / 2, ' ') + std::string(instructionWidth - cellWidth, ' '), PADDING_INSTRUCTION_Y + i / 2 + 2);
    //printCenteredText(std::string(instruction[i].size(), ' ') + instruction[i + 1] + std::string(instructionWidth - instruction[i].size() - instruction[i + 1].size(), ' '), PADDING_INSTRUCTION_Y + i / 2 + 2);
    i++;
  }
}

void setupInstructionDisplay() {
  PADDING_INSTRUCTION_X = (getWindowWidth() - instructionWidth) / 2;
  PADDING_INSTRUCTION_Y = (getWindowHeight() - instructionHeight) / 2;
}
