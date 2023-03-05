#include "SpriteAnimation.h"
#include "Entity.h"
#include "Sprite.h"

namespace gbln {

    void SpriteAnimation::BeginPlay() {
        ownerSprite = owner->GetComponentOfType<gbln::Sprite>();

        if(ownerSprite == nullptr) {
            LOG_ERROR("trying to create a sprite animation component without a sprite component!");
        }
    }

    void SpriteAnimation::Update(float DeltaTime) {
        gueepo::SpriteAnimation_Update(m_spriteAnim, DeltaTime);

        if(ownerSprite != nullptr) {
            ownerSprite->SetSprite(m_spriteAnim.GetCurrentFrameTextureRegion());
        }
    }

    void SpriteAnimation::AddAnimationFrame(gueepo::TextureRegion *tex, float timeOnFrame) {
        m_spriteAnim.AddAnimationFrame(tex, timeOnFrame);
    }
}