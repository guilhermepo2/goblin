#pragma once
#include <map>
#include "gueepo2d.h"

namespace gbln {
    class ResourceManager {
    public:
        ResourceManager();
        ~ResourceManager();

        void ClearResources();

        void AddTexture(gueepo::string textureId, gueepo::Texture* tex);
        void AddTexture(gueepo::string textureId, gueepo::string textureFilepath);
        gueepo::Texture* GetTexture(std::string textureId);
        bool ContainsTexture(std::string textureId);

        void AddTextureRegion(gueepo::string textureId, gueepo::TextureRegion* tex);
        gueepo::TextureRegion* GetTextureRegion(std::string textureRegionId);

        void AddFontSprite(gueepo::string textureId, gueepo::FontSprite* fontSprite);
        void AddFontSpriteFromPath(gueepo::string textureId, int fontSize, gueepo::string filepath);
        gueepo::FontSprite* GetFontSprite(std::string textureId);
    private:
        std::map<std::string, gueepo::Texture*> m_Textures;
        std::map<std::string, gueepo::TextureRegion*> m_textureRegions;
        std::map<std::string, gueepo::Font*> m_fontFiles;
        std::map<std::string, gueepo::FontSprite*> m_fontSprites;
        // todo: scene files...
        // todo: UI files...
        // todo: sounds...

    };
}
