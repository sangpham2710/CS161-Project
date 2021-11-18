#ifndef WELCOME_H_INCLUDED
#define WELCOME_H_INCLUDED

#include <string>
#include <vector>

const std::vector<std::string> welcomeOptions = {"New Game", "Continue Game", "How to Play",
                                                 "Leaderboard", "Exit"};

int Welcome();
void displayWelcomeOptions(const int& oldWelcomeOption);
int handleWelcomeOptions(const int& oldWelcomeOption);

#endif  // WELCOME_H_INCLUDED
