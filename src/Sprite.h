#pragma once
#include "gueepo2d.h"

#include "Component.h"
#include "Transform.h"
#include "Entity.h"

namespace gbln {
    class Sprite : public Component {
    public:
        // todo: have to find a way to make it easier to make it so it can be a texture or a texture region
        gueepo::Texture* sprite;

        Sprite() : sprite(nullptr) {}
        Sprite(gueepo::Texture* tex) : sprite(tex) {}

        void Render() override {
            if(owner->HasComponentOfType<Transform>() && sprite != nullptr) {
                Transform* transform = owner->GetComponentOfType<Transform>();

                gueepo::math::mat4 transformMatrix =
                        gueepo::math::mat4::CreateScale(sprite->GetWidth() * transform->scale) *
                        gueepo::math::mat4::CreateRotation(transform->rotation) *
                        gueepo::math::mat4::CreateTranslation(transform->position);

                gueepo::Renderer::Draw(
                        sprite,
                        transformMatrix * gueepo::math::mat4::m4Identity,
                        gueepo::math::vec2::Zero,
                        gueepo::math::vec2::One,
                        gueepo::Color(1.0f, 1.0f, 1.0f, 1.0f)
                        );
            }
        }
    };
}