#include "Factory.h"

namespace gbln {

    bool Factory::LoadResourceFile(ResourceManager *rm, gueepo::string filepath) {
        gueepo::json resourcesFile(filepath.c_str());

        if(!resourcesFile.IsValid()) {
            return false;
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
                    rm->AddTexture(textureId.c_str(), texturePath.c_str());
                }
            }
        }

        return true;
    }

    bool Factory::LoadTextureRegions(ResourceManager *rm, gueepo::string filepath) {
        gueepo::json textureAtlasObject(filepath.c_str());

        if(!textureAtlasObject.IsValid()) {
            return false;
        }

        gueepo::json internalTextureAtlasJson;
        textureAtlasObject.GetJsonObject("TextureAtlas", internalTextureAtlasJson);
        std::string textureName;
        std::string texturePath;
        internalTextureAtlasJson.GetString("textureName", textureName);
        internalTextureAtlasJson.GetString("texturePath", texturePath);
        gueepo::Texture* textureAtlas = nullptr;
        if(!rm->ContainsTexture(textureName)) {
            rm->AddTexture(textureName.c_str(), texturePath.c_str());
        }
        textureAtlas = rm->GetTexture(textureName);

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
                rm->AddTextureRegion(name.c_str(), texReg);
            }
        }

        return true;
    }

    bool Factory::LoadUIElement(gueepo::string filepath) {
        return false;
    }

    bool Factory::LoadEntity(gueepo::string filepath) {
        return false;
    }
}