#pragma once
#include "Component.h"

namespace gbln {

    class Sprite;

    class SpriteAnimation : public Component {
    public:

        void BeginPlay() override;
        void Update(float DeltaTime) override;
        void AddAnimationFrame(gueepo::TextureRegion* tex, float timeOnFrame);

    private:
        gueepo::SpriteAnimation m_spriteAnim;
        Sprite* ownerSprite = nullptr;
    };
}