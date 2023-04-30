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

        void Update(float deltaTime);
        void LoadResources(ResourceManager* rm, GameWorld* gw);
        void Render();

        gueepo::OrtographicCamera* cameraReference;

    protected:
        ResourceManager* m_rm;
        GameWorld* m_gw;

    };
}