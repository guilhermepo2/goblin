#include "Game.h"
static gueepo::Tilemap* skiTilemap = nullptr;

namespace gbln {
    
	Game::Game() {}
	Game::~Game() {}

	void Game::LoadResources(ResourceManager* rm, GameWorld* gw) {
		gbln::Factory::LoadResourceFile(rm, "./assets/resources.json");
		gueepo::Texture* tinySkiTilemap = rm->GetTexture("kenney_tiny_ski");
		skiTilemap = gbln::Factory::CreateTilemapFromFile(tinySkiTilemap, "./assets/gameplay_map.json");
		gbln::Factory::LoadEntity(gw, rm, "./assets/ski_char_entity.json");
	}

	void Game::Render() {
		// Drawing the tilemap (visualization purposes)
		// #this should be moved to a tilemap component?? how will that work?!
		for (int i = 0; i < skiTilemap->GetNumberOfLayers(); i++) {
			gueepo::TilemapLayer* layer = skiTilemap->GetLayer(i);

			for (int j = 0; j < layer->data.size(); j++) {
				gueepo::Renderer::Draw(
					layer->data[j]->texture,
					((layer->data[j]->x * skiTilemap->GetTileWidth()) * 2) - 285,
					(layer->data[j]->y * skiTilemap->GetTileHeight()) * 2 - 165,
					(skiTilemap->GetTileWidth()) * 2,
					(skiTilemap->GetTileHeight()) * 2
				);
			}
		}
	}

}