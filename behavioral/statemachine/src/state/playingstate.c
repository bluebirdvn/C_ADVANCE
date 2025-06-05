#include "playingstate.h"
#include "musicplayer.h"
#include "pausingstate.h"
#include "stoppingstate.h"
#include <stdio.h>
#include <stdlib.h>

static void pressPlay(PlayerState *state) {
    printf("Press playing button.\n");
    printf("Music player in Playing mode.\n");
}

static void pressPause(PlayerState *state) {
    printf("Press pausing button.\n");
    state->player->ChangeState(state->player, createPausingState());

}

static void pressStop(PlayerState *state) {
    printf("Press Stopping button.\n");
    state->player->ChangeState(state->player, createStoppingState());
    
}

static void setContext(PlayerState *state, MusicPlayer *player) {
    state->player = player;
}

PlayerState *createPlayingState() {
    PlayerState *State = (PlayerState*)malloc(sizeof(PlayerState));
    if (State == NULL) { 
            return NULL;
    }
    State->pressPause = pressPause;
    State->pressPlay = pressPlay;
    State->pressStop = pressStop;
    State->setContext = setContext;
    State->player = NULL;
    return State;
}