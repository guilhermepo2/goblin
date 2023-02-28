#pragma once
#include "gueepo2d.h"

namespace gbln {
    class Entity;

    class Component {
    public:
        Entity* owner;

        Component() {}
        virtual ~Component() {}
        virtual void Initialize() {}
        virtual void BeginPlay() {}
        virtual bool ProcessInput(const gueepo::InputState& CurrentInputState) { return false; }
        virtual void Update(float DeltaTime) {}
        virtual void Render() {}
        virtual void Destroy() {}
    };
}