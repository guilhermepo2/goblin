#pragma once
#include "gueepo2d.h"
#include "Component.h"

namespace gbln {
    class Transform : public Component {
    public:
        gueepo::math::vec2 position;
        float rotation;
        gueepo::math::vec2 scale;

        Transform(const gueepo::math::vec2& _position, float _rotation, const gueepo::math::vec2& _scale) :
            position(_position), rotation(_rotation), scale(_scale) {
        }

        void Translate(const gueepo::math::vec2& deltaMovement) {
            position += deltaMovement;
        }
    };
}