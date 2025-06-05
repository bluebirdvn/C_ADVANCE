#ifndef _PLAYERSTATE_H
#define _PLAYERSTATE_H
#include "musicplayer.h"

struct MusicPlayer;
typedef struct MusicPlayer MusicPlayer;

typedef struct PlayerState {
    struct MusicPlayer *player;

    void(*pressPlay)(struct PlayerState *state);
    void(*pressPause)(struct PlayerState *state);
    void(*pressStop)(struct PlayerState *state);
    void(*setContext)(struct PlayerState *state, MusicPlayer *player);
}PlayerState;

#endif