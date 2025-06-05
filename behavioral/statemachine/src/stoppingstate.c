#include <stdlib.h>
#include <stdio.h>
#include "pausingstate.h"
#include "stoppingstate.h"
#include "playingstate.h"
static void pressPause(PlayerState *State) {
    printf("Pressing pause button.\n");
    printf("Can't Change to Pause State.\n");
}

static void pressPlay(PlayerState *State) {
    printf("Pressing Play button.\n");
    State->player->ChangeState(State->player, createPlayingState());
}

static void pressStop(PlayerState *State) {
    printf("Pressing Stop button.\n");
    printf("Music player in Stopping mode.\n");
}

static void setContext(PlayerState *State, MusicPlayer *Player) {
    State->player = Player;
}

PlayerState *createStoppingState() {
    PlayerState *State = (PlayerState*)malloc(sizeof(PlayerState));
    if (State == NULL) {
        return NULL;
    }State->pressPause = pressPause;
    State->pressPlay = pressPlay;
    State->pressStop = pressStop;
    State->setContext = setContext;
    State->player = NULL;
    return State;
}