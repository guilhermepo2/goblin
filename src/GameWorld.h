#pragma once;
#include "gueepo2d.h"

namespace gbln {
    class Entity;

    class GameWorld {
    public:
        void BeginPlay();
        bool ProcessInput(const gueepo::InputState& CurrentInputState);
        void Update(float DeltaTime);
        void Render();
        void Destroy();

        inline int GetEntitiesCount() const { return m_entities.size(); }
        inline bool HasEntities() const { return m_entities.size() > 0; }
        Entity* AddEntity(const std::string& EntityName);
        Entity* GetEntityByName(const std::string& EntityName);
    private:
        gueepo::vector<Entity*> m_entities;
    };
}