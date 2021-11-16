#ifndef MAIN_UTILS_H_INCLUDED
#define MAIN_UTILS_H_INCLUDED

#include <chrono>
#include <functional>
#include <string>

void setupWindow();
void resetConsoleScreen();
int getStartPositionOfACenteredText(const int &textSize);
int printCenteredText(const std::string &text, const int &coordY);
void printColoredTextWrapper(std::function<void()> func,
                             const int &backgroundColor, const int &textColor);
int mod(const int &a, const int &b);
std::string intToString(const long long &number);
long long getTimeDiff(
    std::chrono::high_resolution_clock::time_point timePoint1,
    std::chrono::high_resolution_clock::time_point timePoint2);

#endif  // MAIN_UTILS_H_INCLUDED
