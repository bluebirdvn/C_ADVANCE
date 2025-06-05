#include "musicplayer.h"
#include "playerstate.h"
#include "playingstate.h"
#include "pausingstate.h"
#include "stoppingstate.h"


int main() {
    MusicPlayer *Player = createMusicPlayer(createStoppingState());
    Player->ClickPausing(Player);
    Player->ClickPlaying(Player);
    Player->ClickStopping(Player);
    Player->ClickStopping(Player);
    Player->ClickPausing(Player);
    Player->ClickPlaying(Player);
    Player->ClickStopping(Player);
    destroyMusicPlayer(Player);
    return 0;


}