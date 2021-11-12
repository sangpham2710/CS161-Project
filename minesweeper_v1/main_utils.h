#ifndef MAIN_UTILS_H_INCLUDED
#define MAIN_UTILS_H_INCLUDED

#include <functional>
#include <string>

void setupWindow();
void resetConsoleScreen();
int getStartPositionOfACenteredText(const int &textSize);
int printCenteredText(const std::string &text, const int &coordY);
void printColoredTextWrapper(std::function<void()> func,
                             const int &backgroundColor, const int &textColor);
int mod(const int &a, const int &b);
std::string intToString(const int &number);

#endif  // MAIN_UTILS_H_INCLUDED
