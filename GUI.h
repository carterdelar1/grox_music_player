#ifndef GUI_H
#define GUI_H

#include <ncurses.h>
#include <string>
#include <vector>
#include <signal.h>

using command_flag = int; // Command flags are communicated as octal numbers.

void cleanup(int sig);

struct LABEL {
    int y, x;
    std::string text;
    command_flag action = 00000; // Optional function pointer for label action
};



// class List {
//     private:
//         int y, x;
//         int width, visibleItems;
//     public:

// };

// class Page {
//     private:
//         std::vector<LABEL> labels;
//         int width, height;

//         WINDOW* win = nullptr;
//     public:
//         Page(int width, int height);

//         void addLabel(int y, int x, std::string text, void (*action)() = nullptr);

//         void drawLabels();

//         void redrawLabel(LABEL &label, bool inverted = false);

//         void drawPage();

// };

#endif