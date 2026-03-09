#include "musicplayer.h"
#include <iostream>

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
        const char* song_path = mpd_song_get_uri(song);

        cout << song_index << ". " << song_path << endl;

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
