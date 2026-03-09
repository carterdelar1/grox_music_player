#include "musicplayer.h"
#include <mpd/status.h>
#include <iostream>
#include <vector>

using namespace std;

MusicPlayer::MusicPlayer() {
    conn = mpd_connection_new(NULL, 0, 0);
}

MusicPlayer::~MusicPlayer() {
    if (conn) mpd_connection_free(conn);
}

void MusicPlayer::setDirectory(const string& subDirectory) {
    if (!conn) return;

    mpd_run_clear(conn);

    if (mpd_run_add(conn, subDirectory.c_str())) {
        cout << "Loaded directory: " << subDirectory << endl;
    }
    else {
        cerr << "Can't find directory: " << subDirectory << endl;
    }
}

void MusicPlayer::addDirectory(const string& subDirectory) {
    if (!conn) return;

    if (mpd_run_add(conn, subDirectory.c_str())) {
        cout << "Added to queue: " << subDirectory << endl;
    }
    else {
        cerr << "Can't find directory: " << subDirectory << endl;
    }
}

string MusicPlayer::getSongTitleArtist_FromPath(const string& path) {
    if (!conn) return "Error: No Connection";

    if (!mpd_send_list_meta(conn, path.c_str())) {
        return "Error: Can't find path.";
    }

    struct mpd_song* song;

    // sets result as path so it has output if it can't correctly pull
    // info from the track
    string output = path;
    bool gotPath = true;

    while ((song = mpd_recv_song(conn)) != NULL) {
        if (gotPath) {
            const char* title = mpd_song_get_tag(song, MPD_TAG_TITLE, 0);
            const char* artist = mpd_song_get_tag(song, MPD_TAG_ARTIST, 0);
            const char* album = mpd_song_get_tag(song, MPD_TAG_ALBUM, 0);

            if (title && artist) {
            output = string(artist) + " - " + string(title);
            if (album) {
                output += " (" + string(album) + ")";
            }
        }
        gotPath = false;
        }
        mpd_song_free(song);
    }
    mpd_response_finish(conn);
    return output;
        

}

string MusicPlayer::getSongTitleArtist_FromStruct(struct mpd_song* song) {
    if (!song) return "Unknown Song";

    const char* title = mpd_song_get_tag(song, MPD_TAG_TITLE, 0);
    const char* artist = mpd_song_get_tag(song, MPD_TAG_ARTIST, 0);
    const char* album = mpd_song_get_tag(song, MPD_TAG_ALBUM, 0);

    string output;

    if (title && artist) {
            output = string(artist) + " - " + string(title);
            if (album) {
                output += " (" + string(album) + ")";
            }
            return output;
        }


    return mpd_song_get_uri(song);
}

string MusicPlayer::getSongTitle(const string& path) {
    if (!conn) return "Error: No Connection";

    if (!mpd_send_list_meta(conn, path.c_str())) {
        return "Error: Can't find path.";
    }

    struct mpd_song* song;

    // sets result as path so it has output if it can't correctly pull
    // info from the track
    string output = path;
    bool gotPath = true;

    while ((song = mpd_recv_song(conn)) != NULL) {
        if (gotPath) {
            const char* title = mpd_song_get_tag(song, MPD_TAG_TITLE, 0);
            if (title) {
                output = string(title);
            }
        }
        gotPath = false;
        }
        mpd_song_free(song);
    mpd_response_finish(conn);
    return output;
}

string MusicPlayer::getSongArtist(const string& path) {
    if (!conn) return "Error: No Connection";

    if (!mpd_send_list_meta(conn, path.c_str())) {
        return "Error: Can't find path.";
    }

    struct mpd_song* song;

    // sets result as path so it has output if it can't correctly pull
    // info from the track
    string output = path;
    bool gotPath = true;

    while ((song = mpd_recv_song(conn)) != NULL) {
        if (gotPath) {
            const char* artist = mpd_song_get_tag(song, MPD_TAG_ARTIST, 0);
            if (artist) {
                output = string(artist);
            }
        }
        gotPath = false;
        }
        mpd_song_free(song);
    mpd_response_finish(conn);
    return output;
}

string MusicPlayer::getSongAlbum(const string& path) {
    if (!conn) return "Error: No Connection";

    if (!mpd_send_list_meta(conn, path.c_str())) {
        return "Error: Can't find path.";
    }

    struct mpd_song* song;

    // sets result as path so it has output if it can't correctly pull
    // info from the track
    string output = path;
    bool gotPath = true;

    while ((song = mpd_recv_song(conn)) != NULL) {
        if (gotPath) {
            const char* album = mpd_song_get_tag(song, MPD_TAG_ALBUM, 0);
            if (album) {
                output = string(album);
            }
        }
        gotPath = false;
        }
        mpd_song_free(song);
    mpd_response_finish(conn);
    return output;
}



void MusicPlayer::play() {
    if (conn) {
        mpd_run_play(conn);
        cout << "Playback started!" << endl;
    }
}



void MusicPlayer::pause() {
    if (conn) {
        mpd_run_toggle_pause(conn);
        cout << "Playback paused!" << endl;
    }
}

void MusicPlayer::showQueue() {
    if (!conn) return;

    if (!mpd_send_list_queue_meta(conn)) {
        cerr << "Error: Can't request queue metadata." << endl;
        return;
    }
    
    struct mpd_song* song;
    int song_index = 1;

    cout << "--Queue--" << endl;

    while ((song = mpd_recv_song(conn)) != NULL) {
        cout << song_index << ". " << getSongTitleArtist_FromStruct(song) << endl;

        song_index++;
        mpd_song_free(song);
    }

    if (song_index == 1) {
        cout << "Queue is empty" << endl;
    }
    cout << "---------" << endl;

    mpd_response_finish(conn);
}

void MusicPlayer::clearQueue() {
    if (conn) {
        if (mpd_run_clear(conn)) {
            cout << "Queue cleared!" << endl;
        }
        else {
            cerr << "Error: Can't clear the queue." << endl;
        }
    }
}

vector<string> MusicPlayer::queueToVector_formatted() {
    vector<string> queueVector;

    if (!conn) return queueVector;

    if (!mpd_send_list_queue_meta(conn)) {
        return queueVector;
    }

    struct mpd_song* song;
    while ((song = mpd_recv_song(conn)) != NULL) {
        queueVector.push_back(getSongTitleArtist_FromStruct(song));

        mpd_song_free(song);
    }

    mpd_response_finish(conn);
    return queueVector;
}

vector<string> MusicPlayer::queueToVector_filepath() {
    vector<string> pathVector;

    if (!conn) return pathVector;

    if (!mpd_send_list_queue_meta(conn)) {
        return pathVector;
    }

    struct mpd_song* song;
    while ((song = mpd_recv_song(conn)) != NULL) {
        const char* uri = mpd_song_get_uri(song);

        if (uri) {
            pathVector.push_back(string(uri));
        }

        mpd_song_free(song);
    }

    mpd_response_finish(conn);
    return pathVector;
}

void MusicPlayer::shuffleQueue() {
    if (!conn) return;

    if (mpd_run_shuffle(conn)) {
        cout << "Queue shuffled!" << endl;
    }
    else {
        cerr << "Error: Could not shuffle." << endl;
    }
}

string MusicPlayer::getCurrentSong() {
    if (!conn) return "No Connection";

    struct mpd_song* song = mpd_run_current_song(conn);
    if (!song) return "None";

    string result = getSongTitleArtist_FromStruct(song);
    mpd_song_free(song);
    return result;
}

void MusicPlayer::toggleRepeatQueue() {
    if (!conn) return;

    if (!mpd_send_status(conn)) return;
    struct mpd_status* status = mpd_recv_status(conn);

    if (status) {
        bool currentState = mpd_status_get_repeat(status);
        mpd_status_free(status);
        mpd_response_finish(conn);

        mpd_run_repeat(conn, !currentState);
    }
    else {
        mpd_response_finish(conn);
    }
}

void MusicPlayer::toggleRepeatCurrentSong() {
    if (!conn) return;

    if (!mpd_send_status(conn)) return;
    struct mpd_status* status = mpd_recv_status(conn);

    if (status) {
        bool currentState = mpd_status_get_single(status);
        mpd_status_free(status);
        mpd_response_finish(conn);

        mpd_run_single(conn, !currentState);
    }
    else {
        mpd_response_finish(conn);
    }
}

int MusicPlayer::getCurrentSongLength() {
    if (!conn) return 0;

    int length = 0;
    if (mpd_send_status(conn)) {
        struct mpd_status* status = mpd_recv_status(conn);
        if (status) {
            length = mpd_status_get_total_time(status);
            mpd_status_free(status);
        }
        mpd_response_finish(conn);
    }
    return length;
}

int MusicPlayer::getCurrentSongElapsedTime() {
    if (!conn) return 0;

    int elapsed_time = 0;
    if (mpd_send_status(conn)) {
        struct mpd_status* status = mpd_recv_status(conn);
        if (status) {
            elapsed_time = mpd_status_get_elapsed_time(status);
            mpd_status_free(status);
        }
        mpd_response_finish(conn);
    }
    return elapsed_time;
}

string MusicPlayer::formatTime(int secondsInput) {
    if (secondsInput < 0) return "0:00";

    int minutes = secondsInput / 60;
    int seconds = secondsInput % 60;

    string secondsString = to_string(seconds);

    if (seconds < 10) {
        secondsString = "0" + secondsString;
    }
    return to_string(minutes) + ":" + secondsString;
}

void MusicPlayer::outputSongTimeStatus() {
    int elapsed = getCurrentSongElapsedTime();
    int total = getCurrentSongLength();

    cout << "[" << formatTime(elapsed) << " / " << formatTime(total) << "]" << endl;
}

void MusicPlayer::currentPlaybackStatus() {
    int elapsed = getCurrentSongElapsedTime();
    int total = getCurrentSongLength();

    cout << "Now Playing: " << getCurrentSong() << endl;
    cout << "[" << formatTime(elapsed) << " / " << formatTime(total) << "]" << endl;
}

void MusicPlayer::nextTrack() {
    if (!conn) return;

    if (mpd_run_next(conn)) {
        cout << "Skipped to next track." << endl;
    }
    else {
        cerr << "Error: Couldn't skip to the next track." << endl;
    }
}

void MusicPlayer::previousTrack() {
    if (!conn) return;

    if (mpd_run_previous(conn)) {
        cout << "Went back one track." << endl;
    }
    cerr << "Error: Couldn't go back a track." << endl;
}

void MusicPlayer::skipTime(int seconds) {
    if (!conn) return;

    int songPos = -1;

    if (mpd_send_status(conn)) {
        struct mpd_status* status = mpd_recv_status(conn);
        if (status) {
            songPos = mpd_status_get_song_pos(status);
            mpd_status_free(status);
        }
        mpd_response_finish(conn);
    }

    if (songPos < 0) return;

    int currentTime = getCurrentSongElapsedTime();
    int totalTime = getCurrentSongLength();
    int targetTime = currentTime + seconds;
    
    if (targetTime < 0) targetTime = 0;
    if (totalTime > 0 && targetTime >= totalTime) {
        targetTime = totalTime - 1;
    }
    mpd_run_seek_pos(conn, songPos, targetTime);
}

void MusicPlayer::goToTimeInTrack(int seconds) {
    if (!conn) return;

    int songPos = -1;

    if (mpd_send_status(conn)) {
        struct mpd_status* status = mpd_recv_status(conn);
        if (status) {
            songPos = mpd_status_get_song_pos(status);
            mpd_status_free(status);
        }
        mpd_response_finish(conn);
    }

    if (songPos < 0) return;

    int totalTime = getCurrentSongLength();
    int targetTime = seconds;
    
    if (targetTime < 0) targetTime = 0;
    if (totalTime > 0 && targetTime >= totalTime) {
        targetTime = totalTime - 1;
    }
    mpd_run_seek_pos(conn, songPos, targetTime);
}