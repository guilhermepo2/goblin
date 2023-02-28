#include "Entity.h"

#include "Component.h"

gbln::Entity::Entity(const std::string& inName) : Name(inName) {}

void gbln::Entity::BeginPlay() {
    for(int i = 0; i < m_components.size(); i++) {
        m_components[i]->BeginPlay();
    }
}

bool gbln::Entity::ProcessInput(const gueepo::InputState &CurrentInputState) {
    for(int i = 0; i < m_components.size(); i++) {
        if(m_components[i]->ProcessInput(CurrentInputState)) {
            return true;
        }
    }

    return false;
}

void gbln::Entity::Update(float DeltaTime) {
    for(int i = 0; i < m_components.size(); i++) {
        m_components[i]->Update(DeltaTime);
    }
}

void gbln::Entity::Render() {
    // todo
    // should this just be delegated or should the entity actually check for sprite components and render it?
    // for now let's just delegate it...
    for(int i = 0; i < m_components.size(); i++) {
        m_components[i]->Render();
    }
}

void gbln::Entity::Destroy() {
    for(int i = 0; i < m_components.size(); i++) {
        m_components[i]->Destroy();
    }

    // todo: properly mark this to destroy and deallocate all pointers
    m_bIsActive = false;
}

