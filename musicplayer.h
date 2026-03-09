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

        // adds a given directory to the queue. can also take a path to a single song and add that.
        void addDirectory(const string& subDirectory);

        // takes a file path and returns the .mp3 formatted "Artist - Title (Album)"
        string getSongTitleArtist_FromPath(const string& path);

        // takes the pointer to the song and returns the song formatted "Artist - Title (Album)"
        string getSongTitleArtist_FromStruct(struct mpd_song* song);

        // outputs string with the song's title
        string getSongTitle(const string& path);

        // outputs string with the song's artist name
        string getSongArtist(const string& path);

        // outputs string with the song's album name
        string getSongAlbum(const string& path);

        // begins playback
        void play();

        // pauses playback
        void pause();

        //displays the current queue
        void showQueue();

        //clears the current queue
        void clearQueue();

        // returns a vector that holds the filepaths for all the songs in the current queue in order
        vector<string> queueToVector_filepath();

        // returns a vector that holds the songs in the current queue in order, formatted "Artist - Title (Album)"
        vector<string> queueToVector_formatted();

        // shuffles the queue
        void shuffleQueue();

        // returns the current song, in the format "Artist - Title (Album)"
        string getCurrentSong();

        // toggles on/off the queue looping once it reaches the end
        void toggleRepeatQueue();

        // toggles the current song looping
        void toggleRepeatCurrentSong();

        // returns an integer that's the current song length in seconds
        int getCurrentSongLength();

        // returns an integer that's the current elapsed time of the song in seconds
        int getCurrentSongElapsedTime();

        // takes an amount of seconds and outputs the minutes and seconds in format "Minutes:Seconds"
        string formatTime(int secondsInput);

        // outputs the current songs time status, in format "[ElapsedTime(Minutes:Seconds) / TotalTime(Minutes:Seconds)]"
        void outputSongTimeStatus();

        // returns the current playback status, in format:
        // Now Playing: <Current Song>
        // [TimeElapsed(Minutes:Seconds) / TotalTime(Minutes:Seconds)]
        void currentPlaybackStatus();

        // skips forward one track in the queue
        void nextTrack();

        // goes back one track in the queue
        void previousTrack();

        // takes an amount in seconds and goes forward that amount if positive and back that amount if negative
        void skipTime(int seconds);

        // takes an amount of time in seconds and goes to that point in the track
        void goToTimeInTrack(int seconds);
};