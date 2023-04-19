//
// Game.h
// The role of a game class is to isolate game-specific things, and "goblin engine" specific things
//

#pragma once
#include "goblin.h"

namespace gbln {
    class Game {
    public:
        Game();
        ~Game();

        void LoadResources(ResourceManager* rm, GameWorld* gw);
        void Render();

    protected:
    private:

    };
}