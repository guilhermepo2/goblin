#include "Factory.h"

#include "Transform.h"
#include "Sprite.h"
#include "LuaComponent.h"

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

        // (2) Loading Fonts
        gueepo::json fontsObject;
        resourcesFile.GetArray("fonts", fontsObject);
        if(fontsObject.IsArray()) {
            for(int i = 0; i < fontsObject.GetArraySize(); i++) {
                gueepo::json tempObject;
                fontsObject.GetObjectInArray(i, tempObject);
                if(tempObject.IsValid()) {
                    std::string fontId;
                    int fontSize;
                    std::string fontPath;
                    tempObject.GetString("id", fontId);
                    tempObject.GetInt("fontSize", fontSize);
                    tempObject.GetString("path", fontPath);
                    rm->AddFontSpriteFromPath(fontId.c_str(), fontSize, fontPath.c_str());
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

    bool Factory::LoadUIElement(gueepo::UIManager* um, ResourceManager* rm, gueepo::string filepath) {
        gueepo::json uiObject(filepath.c_str());

        if(!uiObject.IsValid()) {
            return false;
        }

        gueepo::json labels;
        uiObject.GetArray("labels", labels);
        if(labels.IsArray()) {
            for(int i = 0; i < labels.GetArraySize(); i++) {
                gueepo::json label;
                labels.GetObjectInArray(i, label);
                std::string text, fontSpriteId;
                int x, y;
                label.GetString("text", text);
                label.GetString("fontSpriteId", fontSpriteId);
                label.GetInt("x", x);
                label.GetInt("y", y);

                gueepo::Label* uiLabel = new gueepo::Label(text.c_str(), rm->GetFontSprite(fontSpriteId));
                uiLabel->SetPosition(gueepo::math::vec2(x, y));

                std::string alignment;
                if(label.GetString("alignment", alignment)) {
                    if(alignment == "center") {
                        uiLabel->SetAlignment(gueepo::ALIGNMENT::CENTER);
                    } else if(alignment == "left") {
                        uiLabel->SetAlignment(gueepo::ALIGNMENT::LEFT);
                    } else if(alignment == "right") {
                        uiLabel->SetAlignment(gueepo::ALIGNMENT::RIGHT);
                    }
                }

                um->Push(uiLabel);
            }
        }

        return true;
    }

    gbln::Entity *Factory::LoadEntity(GameWorld *gm, ResourceManager* rm, gueepo::string filepath) {
        gueepo::json entityObject(filepath.c_str());

        if(!entityObject.IsValid()) {
            return nullptr;
        }

        std::string entityName;
        entityObject.GetString("name", entityName);
        gbln::Entity* entity = gm->AddEntity(entityName);

        gueepo::json componentsObject;
        entityObject.GetArray("components", componentsObject);
        if(componentsObject.IsArray()) {
            for(int i = 0; i < componentsObject.GetArraySize(); i++) {
                gueepo::json component;
                gueepo::json properties;

                componentsObject.GetObjectInArray(i, component);
                component.GetJsonObject("properties", properties);

                std::string componentType;
                component.GetString("type", componentType);

                if(componentType == "transform") {
                    int x, y, rot, scale_x, scale_y;
                    properties.GetInt("x", x);
                    properties.GetInt("y", y);
                    properties.GetInt("rotation", rot);
                    properties.GetInt("scale_x", scale_x);
                    properties.GetInt("scale_y", scale_y);
                    entity->AddComponent<Transform>(gueepo::math::vec2(x, y), rot, gueepo::math::vec2(scale_x, scale_y));
                } else if(componentType == "sprite") {
                    std::string textureId;
                    properties.GetString("textureId", textureId);
                    entity->AddComponent<Sprite>(rm->GetTexture(textureId));
                } else if(componentType == "lua") {
                    std::string path;
                    properties.GetString("path", path);
                    entity->AddComponent<LuaComponent>(path.c_str());
                }
            }
        }

        return entity;
    }
}