#include <iostream>
#include <string>
using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cout << "Usage:\n./music_player [.mp3 file path]\n";
        return -1;
    }

    cout << "Song choice: " << argv[1] << '\n';

}