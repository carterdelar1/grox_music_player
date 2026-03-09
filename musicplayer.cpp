#include "musicplayer.h"
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

string MusicPlayer::getSongNameArtist_FromPath(const string& path) {
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

string MusicPlayer::getSongNameArtist_FromStruct(struct mpd_song* song) {
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
        cout << song_index << ". " << getSongNameArtist_FromStruct(song) << endl;

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
        queueVector.push_back(getSongNameArtist_FromStruct(song));

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