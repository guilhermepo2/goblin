#pragma once
#include "gueepo2d.h"
#include <map>
#include <typeinfo>

namespace gbln {
    class Component;

    class Entity {
    public:
        Entity(const std::string& inName);
        virtual void BeginPlay();
        virtual bool ProcessInput(const gueepo::InputState& CurrentInputState);
        virtual void Update(float DeltaTime);
        virtual void Render();
        virtual void Destroy();

        template<typename T, typename... TArgs>
        T* AddComponent(TArgs&&... Args) {
            T* NewComponent(new T(std::forward<TArgs>(Args)...));
            NewComponent->owner = this;
            m_components.add(NewComponent);
            m_typeInfoToComponent[&typeid(*NewComponent)] = NewComponent;
            NewComponent->Initialize();
            return NewComponent;
        }


        template<typename T>
        bool HasComponentOfType() const {
            return m_typeInfoToComponent.count(&typeid(T));
        }

        template<typename T>
        T* GetComponentOfType() {
            return static_cast<T*>(m_typeInfoToComponent[&typeid(T)]);
        }

        inline bool IsActive() const { return m_bIsActive; }
    public:
        std::string Name;
    protected:
        bool m_bIsActive;
        gueepo::vector<Component*> m_components;
        std::map<const std::type_info*, Component*> m_typeInfoToComponent;
    };
}