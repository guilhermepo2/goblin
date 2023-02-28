#include "GameWorld.h"
#include "Entity.h"

namespace gbln {

    void GameWorld::BeginPlay() {
        for(int i = 0; i < m_entities.size(); i++) {
            m_entities[i]->BeginPlay();
        }
    }

    bool GameWorld::ProcessInput(const gueepo::InputState &CurrentInputState) {
        for(int i = 0; i < m_entities.size(); i++) {
            if(m_entities[i]->ProcessInput(CurrentInputState)) {
                return true;
            }
        }
        return false;
    }

    void GameWorld::Update(float DeltaTime) {
        for(int i = 0; i < m_entities.size(); i++) {
            m_entities[i]->Update(DeltaTime);
        }
    }

    void GameWorld::Render() {
        for(int i = 0; i < m_entities.size(); i++) {
            m_entities[i]->Render();
        }
    }

    void GameWorld::Destroy() {
        for(int i = 0; i < m_entities.size(); i++) {
            m_entities[i]->Destroy();
        }

        // todo: free all entities?
    }

    Entity* GameWorld::AddEntity(const std::string &EntityName) {
        Entity* NewEntity = new Entity(EntityName);
        m_entities.add(NewEntity);
        return NewEntity;
    }

    Entity *GameWorld::GetEntityByName(const std::string &EntityName) {
        for(int i = 0; i < m_entities.size(); i++) {
            if(m_entities[i]->Name == EntityName) {
                return m_entities[i];
            }
        }

        return nullptr;
    }
}