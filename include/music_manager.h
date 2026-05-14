
#ifndef MUSIC_MANAGER_H
#define MUSIC_MANAGER_H

typedef enum {
    MUSIC_TRACK_1 = 1,
    MUSIC_TRACK_2 = 2,
    MUSIC_TRACK_3 = 3,
    MUSIC_TRACK_NONE = 0
} MusicTrackID;

void music_system_init(void);

void music_system_play_track(MusicTrackID track);

void music_system_stop(void);

void music_system_pause(void);

void music_system_resume(void);

void music_system_set_volume(float volume);

void music_system_close(void);

#endif
