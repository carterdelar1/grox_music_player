#include <mpd/client.h>
#include <string>
using namespace std;

class MusicPlayer {
    private:
        struct mpd_connection* conn;

    public:
        MusicPlayer();
        ~MusicPlayer();

        // wipes the current directory and sets the input folder
        // as the base directory
        void setDirectory(const string& subDirectory);

        // adds a given directory to the queue
        void addDirectory(const string& subDirectory);


        void play();
        void pause();
        void showQueue();
        void clearQueue();
        
};