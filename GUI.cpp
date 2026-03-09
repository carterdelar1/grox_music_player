#include <ncurses.h>
#include <string>
#include <vector>
#include <signal.h>

void cleanup(int sig) {
    endwin();
    exit(0); 
}

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
        Page(int width, int height) {
            this->width = width;
            this->height = height;

            win = newwin(height, width, 0, 0);
        }

        void addLabel(int y, int x, std::string text, void (*action)() = nullptr) {
            labels.push_back({y, x, text});
        }

        void drawLabels() {
            for (const auto& label : labels) {
                mvwprintw(win, label.y, label.x, "%s", label.text.c_str());
            }
        }

        void redrawLabel(LABEL &label, bool inverted = false) {
            if (inverted) {
                if (has_colors()) {
                    wattron(win, A_REVERSE); // Invert colors for the label
                }
            }    
            mvwprintw(win, label.y, label.x, "%s", label.text.c_str());
        }

        void drawPage() {
            wrefresh(win);
        }


};



int main() {
// Setup
    // Set up signal handlers to ensure clean exit.
    signal(SIGINT, cleanup);  // Handles Ctrl+C
    signal(SIGTERM, cleanup); // Handles kill commands
    
    initscr(); // Initialize the ncurses library
    curs_set(0); // Hides cursor

    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax); // Get the size of the terminal
    WINDOW* testWin = newwin(yMax, xMax, 0, 0); // Create the main window.
    
// End setup

    

    box(testWin, 0, 0); // Draw a box around the window
    //wrefresh(testWin); // Refresh the window to show the box

    if (has_colors()) { // Check if terminal supports color
        start_color();  // Enable color usage
        
        init_pair(1, COLOR_GREEN, COLOR_BLACK); 
        wattron(testWin, COLOR_PAIR(1)); // Set the color pair for text
        wbkgd(testWin, COLOR_PAIR(1)); // Set background color for the window
    }
    //refresh(); // Refresh the screen to show the changes

// Constants definitions
    std::string title = "Grox Music Player";



// End constants definitions
    mvwprintw(testWin, 1, (xMax/2-title.length()/2), "%s",title.c_str()); // Print the title at the top center of the window
    
    wrefresh(testWin); // Refresh the window to show the text
    
    wgetch(testWin); // Wait for user input
    endwin(); // End ncurses mode
    return 0;
}

