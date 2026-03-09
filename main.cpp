#include <mpd/client.h>
#include <iostream>
#include "musicplayer.h"
#include <cstdlib>

using namespace std;

void GMP_MENU() {
    cout << "1. Set Directory (Load Songs)" << endl;
    cout << "2. Add Directory (append to queue)" << endl;
    cout << "3. Play" << endl;
    cout << "4. Pause" << endl;
    cout << "5. Display Queue" << endl;
    cout << "6. Clear Queue" << endl;
    cout << "q. Quit" << endl;
    cout << "Input: ";
}

void clear() {
    system("clear");
}

int main() {
    MusicPlayer GMP;
    string input;
    
    while (true) {
        GMP_MENU();
        cin >> input;

        if (input == "1") {
            string folder;
            cout << "Enter sub-directory for music: ";
            cin.ignore();
            getline(cin, folder);
            GMP.setDirectory(folder);
            clear();
        }

        if (input == "2") {
            string folder;
            cout << "Enter sub-directory to be added to queue: ";
            cin.ignore();
            getline(cin, folder);
            GMP.addDirectory(folder);
            clear();
        }

        else if (input == "3") {
            clear();
            GMP.play();
        }
        else if (input == "4") {
            clear();
            GMP.pause();
        }

        else if (input == "5") {
            clear();
            GMP.showQueue();
        }
        
        else if (input == "6") {
            clear();
            GMP.clearQueue();
        }

        else if (input == "q") {
            clear();
            break;
        }

        else {
            cout << "Please input correctly." << endl;
        }

        
    }

    return 0;
    
}