#!/bin/bash

sudo apt update

sudo apt install -y build-essential cmake mpd libmpdclient-dev pkg-config libncurses-dev

mkdir -p "$HOME/.config/mpd/playlists"

cat <<music_config > mpd.conf
#writes it to your directory with music
music_directory     "/mnt/c/Users/cjcra/Documents/personal/music"

playlist_directory  "$HOME/.config/mpd/playlists"
db_file             "$HOME/.config/mpd/database"
log_file            "$HOME/.config/mpd/log"
pid_file            "$HOME/.config/mpd/pid"
state_file          "$HOME/.config/mpd/state"

bind_to_address     "127.0.0.1"
port                "6600"

audio_output {
    type            "pulse"
    name            "WSL Audio"
}
music_config


