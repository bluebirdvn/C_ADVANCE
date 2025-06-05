#include "pausingstate.h"
#include "stoppingstate.h"
#include "playingstate.h"
#include "playerstate.h"
#include "musicplayer.h"

#include <stdio.h>
#include <stdlib.h>


static void pressPause(PlayerState *state) {
    printf("Pressing Pause button.\n");
    printf("Music player in Pausing mode.\n");
}
static void pressPlay(PlayerState *state) {
    printf("Pressing Play button.\n");
    state->player->ChangeState(state->player, createPlayingState());
}
static void pressStop(PlayerState *state) {
    printf("Pressing Stop button.\n");
    state->player->ChangeState(state->player, createStoppingState());
}
static void setContext(PlayerState *state, MusicPlayer *player){
    state->player = player;
}
PlayerState *createPausingState() {
    PlayerState *state = (PlayerState*)malloc(sizeof(PlayerState));
    if (state == NULL) {
        return NULL;
    }
    state->pressPause = pressPause;
    state->pressPlay = pressPlay;
    state->pressStop = pressStop;
    state->setContext = setContext;
    state->player = NULL;
    return state;
}