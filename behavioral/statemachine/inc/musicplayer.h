#ifndef _MUSICPLAYER_H
#define _MUSICPLAYER_H

#include "playerstate.h"

struct PlayerState;
typedef struct MusicPlayer {
    struct PlayerState *CurrentState;

    void(*ClickPlaying)(struct MusicPlayer *player);
    void(*ClickPausing)(struct MusicPlayer *player);
    void(*ClickStopping)(struct MusicPlayer *player);
    void(*ChangeState)(struct MusicPlayer *player, struct PlayerState *state);
}MusicPlayer;

MusicPlayer *createMusicPlayer(struct PlayerState *initialState);
void destroyMusicPlayer(MusicPlayer *player);


#endif