#include <mpd/client.h>
#include <string>
#include <vector>
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

        string getSongNameArtist_FromPath(const string& path);

        string getSongNameArtist_FromStruct(struct mpd_song* song);



        void play();
        void pause();
        void showQueue();
        void clearQueue();

        vector<string> queueToVector_filepath();

        vector<string> queueToVector_formatted();

        void shuffleQueue();

        
        
};