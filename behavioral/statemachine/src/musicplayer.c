#include "musicplayer.h"
#include <stdlib.h>

void ClickPlay(MusicPlayer *player) {
    if (player->CurrentState && player->CurrentState->pressPlay){
            player->CurrentState->pressPlay(player->CurrentState);
    }
}

void ClickPause(MusicPlayer *player) {
    if (player->CurrentState && player->CurrentState->pressPause) {
        player->CurrentState->pressPause(player->CurrentState);
    }
}

void ClickStop(MusicPlayer *player) {
    if (player->CurrentState && player->CurrentState->pressStop) {
        player->CurrentState->pressStop(player->CurrentState);
    }
}

void ChangeState(MusicPlayer *player, PlayerState *NewState) {
    if (player->CurrentState) {
        free(player->CurrentState);
    }
    player->CurrentState = NewState;
    if(player->CurrentState && player->CurrentState->setContext) {
        player->CurrentState->setContext(player->CurrentState, player);
    }

}


MusicPlayer *createMusicPlayer(PlayerState *initialState) {
    MusicPlayer *player = (MusicPlayer*)malloc(sizeof(MusicPlayer));
    if (player == NULL) {
        return NULL;
    }
    player->CurrentState = initialState;
    player->ClickPausing = ClickPause;
    player->ClickPlaying = ClickPlay;
    player->ClickStopping = ClickStop;
    player->ChangeState = ChangeState;
    if(initialState && initialState->setContext) {
        initialState->setContext(initialState, player);
    }
    return player;
}

void destroyMusicPlayer(MusicPlayer *player) {
    if (player) {
        if (player->CurrentState) {
            free(player->CurrentState);
        }
        free(player);
    }
}
















