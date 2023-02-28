#pragma once
#include <map>
#include "gueepo2d.h"

namespace gbln {
    class ResourceManager {
    public:
        ResourceManager();
        ~ResourceManager();

        void LoadResource(gueepo::string filepath);
        void ClearResources();

        void AddTexture(gueepo::string textureId, gueepo::Texture* tex);
        void AddTexture(gueepo::string textureId, gueepo::string textureFilepath);
        gueepo::Texture* GetTexture(std::string textureId);
    private:
        std::map<std::string, gueepo::Texture*> m_Textures;
        // todo: fonts...
        // todo: scene files...
        // todo: UI files...
        // todo: sounds...
    };
}
