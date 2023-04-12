#include "Factory.h"

#include "Transform.h"
#include "Sprite.h"
#include "SpriteAnimation.h"
#include "LuaComponent.h"

namespace gbln {

    // *************************************************************************************************************
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

        // (2) Loading Texture Regions
        gueepo::json textureRegionsObject;
        resourcesFile.GetArray("textureRegions", textureRegionsObject);
        if (textureRegionsObject.IsArray()) {
            for (int i = 0; i < textureRegionsObject.GetArraySize(); i++) {
                gueepo::json textureAtlasObject;
                textureRegionsObject.GetObjectInArray(i, textureAtlasObject);
                gueepo::json internalTextureAtlasObject;
                textureAtlasObject.GetJsonObject("TextureAtlas", internalTextureAtlasObject);
                LoadTextureRegions(rm, internalTextureAtlasObject);
            }
        }

        // (3) Loading Fonts
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

    // *************************************************************************************************************
    bool Factory::LoadTextureRegions(ResourceManager *rm, gueepo::string filepath) {
        gueepo::json textureAtlasObject(filepath.c_str());

        if(!textureAtlasObject.IsValid()) {
            return false;
        }

        gueepo::json internalTextureAtlasJson;
        textureAtlasObject.GetJsonObject("TextureAtlas", internalTextureAtlasJson);
        return LoadTextureRegions(rm, internalTextureAtlasJson);
    }

    // *************************************************************************************************************
	bool Factory::LoadTextureRegions(ResourceManager* rm, gueepo::json textureAtlasObject) {
		std::string textureName;
		std::string texturePath;
        textureAtlasObject.GetString("textureName", textureName);
        textureAtlasObject.GetString("texturePath", texturePath);

		gueepo::Texture* textureAtlas = nullptr;
		if (!rm->ContainsTexture(textureName)) {
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
        textureAtlasObject.GetArray("SubTextures", textureRegionsArray);
		if (textureRegionsArray.IsArray()) {
			for (int i = 0; i < textureRegionsArray.GetArraySize(); i++) {
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

    // *************************************************************************************************************
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

    // *************************************************************************************************************
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
                    if (properties.GetString("textureId", textureId)) {
                        entity->AddComponent<Sprite>(rm->GetTexture(textureId));
                    }
                    else if (properties.GetString("textureRegionId", textureId)) {
                        Sprite* spr = entity->AddComponent<Sprite>();
                        spr->SetSprite(rm->GetTextureRegion(textureId));
                    }
                } else if(componentType == "lua") {
                    std::string path;
                    properties.GetString("path", path);
                    entity->AddComponent<LuaComponent>(path.c_str());
                }
                else if (componentType == "sprite_animation") {
                    SpriteAnimation* spriteAnimation = entity->AddComponent<SpriteAnimation>();

                    gueepo::json framesObject;
                    properties.GetArray("frames", framesObject);
                    if (framesObject.IsArray()) {
                        for (int i = 0; i < framesObject.GetArraySize(); i++) {
                            gueepo::json singleFrame;
                            framesObject.GetObjectInArray(i, singleFrame);
                            std::string textureRegionId;
                            int timeOnFrame;

                            singleFrame.GetString("textureRegionId", textureRegionId);
                            singleFrame.GetInt("timeOnFrame", timeOnFrame);

                            float timeOnFrameInS = static_cast<float>(timeOnFrame / 1000.0f);
                            spriteAnimation->AddAnimationFrame(rm->GetTextureRegion(textureRegionId), timeOnFrameInS);
                        }
                    }
                }
            }
        }

        return entity;
    }

    // *************************************************************************************************************
	gueepo::Tilemap* Factory::CreateTilemapFromFile(gueepo::Texture* tilemapTexture, gueepo::string filepath) {
        gueepo::json tilemapObject(filepath.c_str());

        if (!tilemapObject.IsValid()) {
            LOG_ERROR("error opening tilemap file: {0}", filepath);
            return nullptr;
        }

        gueepo::TILEMAP_PARAMS tilemapParams;
		tilemapObject.GetInt("width", tilemapParams.width);
		tilemapObject.GetInt("height", tilemapParams.height);
		tilemapObject.GetInt("tilewidth", tilemapParams.tileWidth);
		tilemapObject.GetInt("tileheight", tilemapParams.tileHeight);
		gueepo::Tilemap* tilemap = new gueepo::Tilemap(tilemapParams);

        // iterating through all layers...
        gueepo::json layersJsonArray;
        tilemapObject.GetArray("layers", layersJsonArray);
        if (layersJsonArray.IsArray()) {
            for (int i = 0; i < layersJsonArray.GetArraySize(); i++) {
                gueepo::json currentLayerObject;
                layersJsonArray.GetObjectInArray(i, currentLayerObject);

				bool visible;
				currentLayerObject.GetBool("visible", visible);
				if (!visible) {
					continue;
				}

                gueepo::TilemapLayer* tilemapLayer = new gueepo::TilemapLayer();
				std::string layerName;
				currentLayerObject.GetString("name", layerName);
				tilemapLayer->layerName = layerName.c_str();

				// Here we do the regular visual processing of the tilemap...
				// Getting the data (the hard part)
                gueepo::json dataArrayObject;
                currentLayerObject.GetArray("data", dataArrayObject);
                if (dataArrayObject.IsArray()) {
                    for (int i = 0; i < dataArrayObject.GetArraySize(); i++) {
                        int contentInteger;
                        dataArrayObject.GetIntAt(i, contentInteger);

                        if (contentInteger == 0) {
                            continue;
                        }

						// ok... now we have the tile number, we have to translate that into texture region...
						// for example... id 0 means (0,0)
						// whereas...	  id 81 means (1, 4) => and the actual (x, y) on the texture is (16, 64), and size is (16,16)
						// so the formula is... (id % width, id / width)
						contentInteger--;
						int tile_x = contentInteger % tilemap->GetWidth();
						int tile_y = contentInteger / tilemap->GetWidth();
						int tileWidth = tilemap->GetTileWidth();
						int tileHeight = tilemap->GetTileHeight();
						int textureRegionX = tile_x * tileWidth;
                        int textureRegionY = tilemapTexture->GetHeight() - ((tile_y + 1) * tileHeight);

                        // #todo: keep track of all the texture regions, so we don't recreate if they are the same ?!
                        // #todo: Sounds like it would be the work of a "tilemap" resource, not a tilemap... Hm... a SpriteSheet? a TextureAtlas?
                        gueepo::TextureRegion* texReg = new gueepo::TextureRegion(tilemapTexture, textureRegionX, textureRegionY, tileWidth, tileHeight);
                        int tilePositionX = i % tilemap->GetWidth();
						// having to invert the Y because we are from the top left on the tilemap
						// so we also have to start pushing on the top left (which is the highest Y)
						int tilePositionY = tilemap->GetHeight() - (i / tilemap->GetWidth());
						gueepo::Tile* tile = new gueepo::Tile();
                        tile->x = tilePositionX;
                        tile->y = tilePositionY;
						tile->texture = texReg;

                        tilemapLayer->data.add(tile);
                    } // for (int i = 0; i < dataArrayObject.GetArraySize(); i++)
                } // if (dataArrayObject.IsArray())

                tilemap->AddLayer(tilemapLayer);
            } // for (int i = 0; i < layersJsonArray.GetArraySize(); i++)
        } // if (layersJsonArray.IsArray())

        return tilemap;
	}

}