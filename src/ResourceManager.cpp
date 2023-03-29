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

        for(auto textureRegion : m_textureRegions) {
            delete textureRegion.second;
        }
        m_textureRegions.clear();

		for (auto fontFile : m_fontFiles) {
			delete fontFile.second;
		}
		m_fontFiles.clear();

        for (auto fontSprite : m_fontSprites) {
            delete fontSprite.second;
        }
        m_fontSprites.clear();
    }

    // --------------------------------------------------------------------------------------
    // --------------------------------------------------------------------------------------
    // Texture
    // --------------------------------------------------------------------------------------
    // --------------------------------------------------------------------------------------
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

    bool ResourceManager::ContainsTexture(std::string textureId) {
        return m_Textures.count(textureId) != 0;
    }

	// --------------------------------------------------------------------------------------
	// --------------------------------------------------------------------------------------
	// Texture Region
	// --------------------------------------------------------------------------------------
	// --------------------------------------------------------------------------------------
    void ResourceManager::AddTextureRegion(gueepo::string textureId, gueepo::TextureRegion *tex) {
        m_textureRegions.emplace(textureId.c_str(), tex);
    }

    void ResourceManager::LoadTextureRegions(gueepo::string filepath) {
        gueepo::json textureAtlasObject(filepath.c_str());

        if(!textureAtlasObject.IsValid()) {
            return;
        }

        gueepo::json internalTextureAtlasJson;
        textureAtlasObject.GetJsonObject("TextureAtlas", internalTextureAtlasJson);
        std::string textureName;
        std::string texturePath;
        internalTextureAtlasJson.GetString("textureName", textureName);
        internalTextureAtlasJson.GetString("texturePath", texturePath);
        gueepo::Texture* textureAtlas = nullptr;
        if(!ContainsTexture(textureName)) {
            AddTexture(textureName.c_str(), texturePath.c_str());
        }
        textureAtlas = GetTexture(textureName);

        // here is the issue. the spritesheet maps as (0,0) being bottom left
        // but the engine loads as (0,0) being the bottom right
        // so we have to convert the y.
        // if y = 0 -> y = textureHeight - spriteHeight
        // if = textureHeight - spriteHeight => y = 0
        // so... y = (textureHeight - spriteHeight) - oldY ? I think that makes sense.
        int textureHeight = textureAtlas->GetHeight();

        gueepo::json textureRegionsArray;
        internalTextureAtlasJson.GetArray("SubTextures", textureRegionsArray);
        if(textureRegionsArray.IsArray()) {
            for(int i = 0; i < textureRegionsArray.GetArraySize(); i++) {
                gueepo::json textureRegion;
                textureRegionsArray.GetObjectInArray(i, textureRegion);

                std::string name;
                int x, y, width, height;
                textureRegion.GetString("_name", name);
                textureRegion.GetInt("_x", x);
                textureRegion.GetInt("_y", y);
                textureRegion.GetInt("_width", width);
                textureRegion.GetInt("_height", height);
                gueepo::TextureRegion* texReg = new gueepo::TextureRegion(textureAtlas, x, (textureHeight - height) - y, width, height);
                AddTextureRegion(name.c_str(), texReg);
            }
        }
    }

    gueepo::TextureRegion *ResourceManager::GetTextureRegion(std::string textureRegionId) {
        gueepo::TextureRegion* tex = nullptr;

        if(m_textureRegions.count(textureRegionId) != 0) {
            tex = m_textureRegions[textureRegionId];
        }

        return tex;
    }

	// --------------------------------------------------------------------------------------
	// --------------------------------------------------------------------------------------
	// Fonts
	// --------------------------------------------------------------------------------------
	// --------------------------------------------------------------------------------------
	void ResourceManager::AddFontSprite(gueepo::string textureId, gueepo::FontSprite* fontSprite) {
        m_fontSprites.emplace(textureId.c_str(), fontSprite);
	}

	void ResourceManager::AddFontSpriteFromPath(gueepo::string textureId, int fontSize, gueepo::string filepath) {
        gueepo::Font* fontFile = nullptr;

        if (m_fontFiles.count(filepath.c_str()) != 0) {
            fontFile = m_fontFiles[filepath.c_str()];
        }
        else {
            fontFile = gueepo::Font::CreateNewFont(filepath.c_str());
            m_fontFiles.emplace(filepath.c_str(), fontFile);
        }

        if (fontFile != nullptr) {
            gueepo::FontSprite* fontSprite = new gueepo::FontSprite(fontFile, fontSize);

            if (fontSprite != nullptr) {
                fontSprite->SetLineGap(fontSize);
                AddFontSprite(textureId, fontSprite);
            }
        }
	}

	gueepo::FontSprite* ResourceManager::GetFontSprite(std::string textureId) {
        gueepo::FontSprite* fontSprite = nullptr;

        if (m_fontSprites.count(textureId) != 0) {
            fontSprite = m_fontSprites[textureId];
        }

        return fontSprite;
	}

}
