#include <ncurses.h>
#include <string>
#include <vector>
#include <signal.h>

void cleanup(int sig);

struct LABEL {
    int y, x;
    std::string text;
    void (*action)() = nullptr; // Optional function pointer for label action
};



class List {
    private:
        int y, x;
        int width, visibleItems;
    public:

};

class Page {
    private:
        std::vector<LABEL> labels;
        int width, height;

        WINDOW* win = nullptr;
    public:
        Page(int width, int height);

        void addLabel(int y, int x, std::string text, void (*action)() = nullptr);

        void drawLabels();

        void redrawLabel(LABEL &label, bool inverted = false);

        void drawPage();

};