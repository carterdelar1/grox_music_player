#include <ncurses.h>
#include <string>
#include <vector>
#include <signal.h>
#include <iostream>
#include "musicplayer.h"
#include "GUI.h"

void cleanup(int sig) {
    endwin();
    exit(0); 
}

/*
LABEL represents some text label on screen.
LABEL can be highlighted and interacted with via methods within class Page.

Fields:
    int y - The y coordinate of the first character of the label
    int x - The x coordinate f the first character of the label
    string text - The text displayed by the label.
    command_flag action - 
*/
// 



class List {
    private:
        int y, x;
        int width, visibleItems;
        std::vector<std::string> songPaths;
        int selectedIndex = 0;
        int topIndex = 0; // Index of the topmost visible item
        int bottomIndex; // Index of the bottommost visible item  

    public:
        List(int y, int x, int width, int visibleItems) {
            this->y = y;
            this->x = x;
            this->width = width;
            this->visibleItems = visibleItems;
            this->bottomIndex = visibleItems - 1; // Initialize bottom index based on visible items

        }
    
        void addItem(const std::string& songPath) {
            songPaths.push_back(songPath);
        }

        void drawList(WINDOW* win) {
            for (int i = 0; i < visibleItems && (topIndex + i) < songPaths.size(); ++i) {
                int index = topIndex + i;
                std::string displayText = songPaths[index].substr(songPaths[index].find_last_of("/\\") + 1); // Extract filename
                if (index == selectedIndex) {
                    wattron(win, A_REVERSE); // Highlight selected item
                    mvwprintw(win, y + i, x, "%-*s", width, displayText.c_str());
                    wattroff(win, A_REVERSE);
                } else {
                    mvwprintw(win, y + i, x, "%-*s", width, displayText.c_str());
                }
            }
        }

        void incrementList() {
            if (selectedIndex < songPaths.size() - 1) {
                selectedIndex++;
                if (selectedIndex > bottomIndex) {
                    topIndex++;
                    bottomIndex++;
                }
            }
        }

        void decrementList() {
            if (selectedIndex > 0) {
                selectedIndex--;
                if (selectedIndex < topIndex) {
                    topIndex--;
                    bottomIndex--;
                }
            }
        }
};


class Page {
    private:
        std::vector<LABEL> highlightables;
        std::vector<LABEL> texts;
        int width, height;

        WINDOW* win;
    public:
        Page(int width, int height) {
            this->width = width;
            this->height = height;

            win = newwin(height, width, 0, 0);

            if (has_colors()) {
                wbkgd(win, COLOR_PAIR(1)); 
            }
        }

        WINDOW* getWindow() {
            return win;
        }

        void addHighlightable(int y, int x, std::string text, command_flag action = 00000) {
            highlightables.push_back({y, x, text, action});
        }

        void addText(int y, int x, std::string text) {
            texts.push_back({y,x,text});
        }

        void editText(int index, std::string newText) {
            if (index >= 0 && index < texts.size()) {
                texts[index].text = newText;
            }
        }

        void drawHighlightables() {
            for (auto& label : highlightables) {
                redrawHighlightable(label, false);
            }
        }

        void drawTexts() {
            for (auto& text : texts) {
                mvwprintw(win, text.y, text.x, "%s", text.text.c_str());
            }
        }

        void redrawHighlightable(LABEL &label, bool inverted = false) {
            if (inverted) {
                if (has_colors()) {
                    wattron(win, COLOR_PAIR(2)); // Invert colors for the label
                }
            }    
            mvwprintw(win, label.y, label.x, "%s", label.text.c_str());
            
            wattron(win, COLOR_PAIR(1));

        }

        void drawPage() {
            //werase(win);
            drawTexts();
            drawHighlightables();
            wrefresh(win);
        }
        
        // Handle user input
        command_flag navigationLoop() {
                keypad(win, TRUE);

                int ch;
                int selectedIndex = 0;
    
                while ((ch = wgetch(win)) != 'q') { // Press 'q' to quit
                    switch (ch) {
                        case KEY_UP:
                            if (selectedIndex > 0) {
                                redrawHighlightable(highlightables[selectedIndex], false); // Unhighlight current
                                selectedIndex--;
                                redrawHighlightable(highlightables[selectedIndex], true); // Highlight new
                                wrefresh(win);
                               
                            }
                            break;
                        case KEY_DOWN:
                            if (selectedIndex < highlightables.size() - 1) {
                                redrawHighlightable(highlightables[selectedIndex], false); // Unhighlight current
                                selectedIndex++;
                                redrawHighlightable(highlightables[selectedIndex], true); // Highlight new
                                wrefresh(win);
                            }
                            break;
                        case '\n': // Enter key
                            werase(win);
                            return highlightables[selectedIndex].action;
                            break;
                    }
                }

                return 00000; // Exit 

        }


};



int main() {
    MusicPlayer player;
// Setup
    // Set up signal handlers to ensure clean exit.
    signal(SIGINT, cleanup);  // Handles Ctrl+C
    signal(SIGTERM, cleanup); // Handles kill commands
    
    initscr(); // Enable the ncurses screen
    curs_set(0); // Hides cursor

    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax); // Get the size of the terminal
    WINDOW* baseWindow = newwin(yMax, xMax, 0, 0); // Create the main window.
    
    box(baseWindow, 0, 0); // Draw a box around the window

    if (has_colors()) { // Check if terminal supports color
        start_color();  // Enable color usage
        
        init_pair(1, COLOR_GREEN, COLOR_BLACK); // Main colors
        init_pair(2, COLOR_BLACK, COLOR_GREEN); // Highlight colors

        wattron(baseWindow, COLOR_PAIR(1)); // Set the color pair for text
        wbkgd(baseWindow, COLOR_PAIR(1)); // Set background color for the window
    }
// End setup

// Constants definitions
    
// End constants definitions

// Create pages
    // Title page
    std::string title = "Grox Music Player";

    Page titleScreen = Page(xMax-2, yMax-2);

    titleScreen.addHighlightable(yMax/2, 3, "Play", 00002);
    titleScreen.addHighlightable(yMax/2 + 2, 3, "Settings", 00003);
    titleScreen.addText(2, xMax/2 - (title.length()/2), title);
    // End title page


    // Play page
    Page playScreen = Page(xMax-2, yMax-2);

    playScreen.addText(10, 3, "Song Name");
    playScreen.addText(12, 3, "Artist Name");
    playScreen.addText(14, 3, "Album Name");
    playScreen.addText(16, 3, "Total Time");
    // End play page


    // Settings page

    // End settings page





// End create pages

    
    noecho();
    //mvwprintw(baseWindow, 1, (xMax/2-title.length()/2), "%s",title.c_str()); // Print the title at the top center of the window
    
    //wrefresh(baseWindow); // Refresh the window to show the text
   

    command_flag command = 00001; // 

    while (command != 00000) {
        switch (command) {
            case 00001:
                titleScreen.drawPage();
                command = titleScreen.navigationLoop();
                break;
            case 00002:
                playScreen.drawPage();
                command = playScreen.navigationLoop();

                player.play();
                break;
            case 00003:
                //settingsScreen.drawPage();
                //settingsScreen.navigationLoop();
                break;
        }
    }
    // titleScreen.navigationLoop();

    //wgetch(titleScreen.getWindow()); // Wait for user input
    endwin(); // End ncurses mode
    return 0;
}

