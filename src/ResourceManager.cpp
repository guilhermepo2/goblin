#include "ResourceManager.h"
#include "gueepo2d.h"

namespace gbln {
    ResourceManager::ResourceManager() {}

    ResourceManager::~ResourceManager() {
        ClearResources();
    }

    void ResourceManager::LoadResource(gueepo::string filepath) {
        gueepo::json resourcesFile(filepath.c_str());

        if(!resourcesFile.IsValid()) {
            return;
        }

        // (1) Loading Textures
        gueepo::json texturesObject;
        resourcesFile.GetArray("textures", texturesObject);
        if(texturesObject.IsArray()) {
            for(int i = 0; i < texturesObject.GetArraySize(); i++) {
                gueepo::json tempObject;
                texturesObject.GetObjectInArray(i, tempObject);
                if(tempObject.IsValid()) {
                    std::string textureId;
                    std::string texturePath;
                    tempObject.GetString("id", textureId);
                    tempObject.GetString("path", texturePath);
                    AddTexture(textureId.c_str(), texturePath.c_str());
                }
            }
        }
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
