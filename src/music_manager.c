
#include "music_manager.h"
#include "simple_logger.h"
#include "gfc_audio.h"

static Mix_Music *track1 = NULL;
static Mix_Music *track2 = NULL;
static Mix_Music *track3 = NULL;
static Mix_Music *current_track = NULL;
static float current_volume = 0.8f;
static int is_playing = 0;

void music_system_init(void) {
    gfc_audio_init(64, 16, 4, 10, 1, 1);

    track1 = gfc_sound_load_music("music/track1.ogg");
    track2 = gfc_sound_load_music("music/track2.ogg");
    track3 = gfc_sound_load_music("music/track3.ogg");

    if (!track1 || !track2 || !track3) {
        slog("Failed to load one or more music tracks");
    } else {
        slog("All 3 music tracks loaded successfully");
    }

    Mix_VolumeMusic((int)(current_volume * MIX_MAX_VOLUME));
}

void music_system_play_track(MusicTrackID track) {
    if (is_playing) {
        Mix_HaltMusic();
    }

    switch(track) {
        case MUSIC_TRACK_1:
            current_track = track1;
            break;
        case MUSIC_TRACK_2:
            current_track = track2;
            break;
        case MUSIC_TRACK_3:
            current_track = track3;
            break;
        default:
            return;
    }

    if (current_track) {
        if (Mix_PlayMusic(current_track, -1) == 0) {
            is_playing = 1;
            slog("Now playing track %d", track + 1);
        } else {
            slog("Failed to play track: %s", SDL_GetError());
        }
    }
}

void music_system_stop(void) {
    if (is_playing) {
        Mix_HaltMusic();
        is_playing = 0;
        current_track = NULL;
        slog("Music stopped");
    }
}

void music_system_pause(void) {
    if (is_playing && Mix_PlayingMusic()) {
        Mix_PauseMusic();
        slog("Music paused");
    }
}

void music_system_resume(void) {
    if (is_playing && Mix_PausedMusic()) {
        Mix_ResumeMusic();
        slog("Music resumed");
    }
}

void music_system_set_volume(float volume) {
    if (volume < 0.0f) volume = 0.0f;
    if (volume > 1.0f) volume = 1.0f;
    current_volume = volume;
    Mix_VolumeMusic((int)(volume * MIX_MAX_VOLUME));
    slog("Music volume set to %.2f", volume);
}

void music_system_close(void) {
    if (track1) Mix_FreeMusic(track1);
    if (track2) Mix_FreeMusic(track2);
    if (track3) Mix_FreeMusic(track3);

    is_playing = 0;

    gfc_sound_clear_all();

    slog("Music system closed");
}
