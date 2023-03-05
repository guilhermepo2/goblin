#pragma once
#include "gueepo2d.h"

#include "Component.h"
#include "Transform.h"
#include "Entity.h"

namespace gbln {
    class Sprite : public Component {
    public:
        // todo: have to find a way to make it easier to make it so it can be a texture or a texture region
        gueepo::TextureRegion* sprite;

        Sprite() : sprite(nullptr) {}
        Sprite(gueepo::Texture* tex) {
            g2dassert(tex != nullptr, "trying to create a texture with a nullptr? use empty constructor instead");
            sprite = new gueepo::TextureRegion(tex, 0, 0, tex->GetWidth(), tex->GetHeight());
        }

        void SetSprite(gueepo::TextureRegion* _sprite) {
            sprite = _sprite;
        }

        void Render() override {
            if(owner->HasComponentOfType<Transform>() && sprite != nullptr) {
                Transform* transform = owner->GetComponentOfType<Transform>();
                gueepo::math::vec2 scale = transform->scale;

                gueepo::Renderer::Draw(
                        sprite,
                        transform->position.x,
                        transform->position.y,
                        sprite->GetWidth() * scale.x,
                        sprite->GetHeight() * scale.y
                        );
            }
        }
    };
}