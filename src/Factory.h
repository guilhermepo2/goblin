//
// Factory.h
//

#pragma once
#include "ResourceManager.h"

namespace gbln {
    class Factory {
    public:
        // Searches for arrays in a json file
        // 1. Searches for an array called "textures", each object on this array should have:
        // "id" (string), and "path" (string)
        static bool LoadResourceFile(ResourceManager* rm, gueepo::string filepath);

        // 1. Search for a "TextureAtlas" object
        // 2. A texture atlas should have: "textureName" (string), "texturePath" (string), and "SubTextures" (array of objects)
        // 3. each subtexture should have:
        // "_name" (string), "_x" (int), "_y" (int), "_width" (int), "_height" (int)
        static bool LoadTextureRegions(ResourceManager* rm, gueepo::string filepath);

        static bool LoadUIElement(gueepo::string filepath);
        static bool LoadEntity(gueepo::string filepath);

        // #todo: load scene (just a bunch of entities?)
        // #todo load entire UI Screen (just a bunch of UI elements?)

    };
}
