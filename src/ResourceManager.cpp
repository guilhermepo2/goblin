#include "ResourceManager.h"
#include "gueepo2d.h"

namespace gbln {
    ResourceManager::ResourceManager() {}

    ResourceManager::~ResourceManager() {
        ClearResources();
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
