#include "ResourceManager.h"
#include "gueepo2d.h"

namespace gbln {
    ResourceManager::ResourceManager() {}

    ResourceManager::~ResourceManager() {
        ClearResources();
    }

    void ResourceManager::LoadResource(gueepo::string filepath) {
        // todo! Is this going to be a json or a lua file?
    }

    void ResourceManager::ClearResources() {
        for(auto texture : m_Textures) {
            delete texture.second;
        }
        m_Textures.clear();
    }

    void ResourceManager::AddTexture(gueepo::string textureId, gueepo::Texture *tex) {
        m_Textures.emplace(textureId.c_str(), tex);
    }

    void ResourceManager::AddTexture(gueepo::string textureId, gueepo::string textureFilepath) {
        gueepo::Texture* tex = gueepo::Texture::Create(textureFilepath.c_str());

        if(tex != nullptr) {
            AddTexture(textureId, tex);
        }
    }

    gueepo::Texture *ResourceManager::GetTexture(std::string textureId) {
        gueepo::Texture* tex = nullptr;

        if(m_Textures.count(textureId) != 0) {
            tex = m_Textures[textureId];
        }

        return tex;
    }
}
