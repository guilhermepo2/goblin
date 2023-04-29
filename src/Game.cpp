#include "Game.h"

// static gueepo::Tilemap* skiTilemap = nullptr;
static gueepo::Tilemap* villageTilemap = nullptr;

namespace gbln {
    
	Game::Game() {}
	Game::~Game() {}

	void Game::Update(float deltaTime) {
		// playerScore += deltaTime;
	}

	void Game::LoadResources(ResourceManager* rm, GameWorld* gw) {
		m_rm = rm;
		m_gw = gw;

		// Old stuff from "Ski Game" - here for sample on how to load stuff...
		// gbln::Factory::LoadResourceFile(rm, "./assets/resources.json");
		// gueepo::Texture* tinySkiTilemap = rm->GetTexture("kenney_tiny_ski");
		// skiTilemap = gbln::Factory::CreateTilemapFromFile(tinySkiTilemap, "./assets/gameplay_map.json");
		// gbln::Factory::LoadEntity(gw, rm, "./assets/ski_char_entity.json");
		//

		// Loading Resources (could be in a file)
		rm->AddTexture("tilemap_dungeon", "./assets/ld53/tilemap_dungeon.png");
		rm->AddTexture("tilemap_town", "./assets/ld53/tilemap_town.png");

		
		gueepo::Texture* tilemapDungeon = rm->GetTexture("tilemap_dungeon");
		gueepo::Texture* tilemapTown = rm->GetTexture("tilemap_town");

		villageTilemap = gbln::Factory::CreateTilemapFromFile(tilemapTown, "./assets/ld53/village4.json");

		// Setting up entities (could be in a file)
		gbln::Entity* deliveryguy = gw->AddEntity("deliveryguy");
		deliveryguy->AddComponent<gbln::Transform>(gueepo::math::vec2(0.0f, 0.0f), 0.0f, gueepo::math::vec2(1.0f, 1.0f));
		gbln::Sprite* spriteComp = deliveryguy->AddComponent<gbln::Sprite>();
		spriteComp->SetSprite(new gueepo::TextureRegion(tilemapDungeon, 16, 48, 16, 16));
		deliveryguy->AddComponent<gbln::LuaComponent>("./assets/ld53/delivery_guy.lua");
	}

	void Game::Render() {
		// Drawing the tilemap (visualization purposes)
		// #this should be moved to a tilemap component?? how will that work?!
		/*
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
		*/

		for (int i = 0; i < villageTilemap->GetNumberOfLayers(); i++) {
			gueepo::TilemapLayer* layer = villageTilemap->GetLayer(i);

			for (int j = 0; j < layer->data.size(); j++) {

				gueepo::Renderer::Draw(
					layer->data[j]->texture,
					((layer->data[j]->x * villageTilemap->GetTileWidth())) - 320,
					(layer->data[j]->y * villageTilemap->GetTileHeight()) - 160,
					(villageTilemap->GetTileWidth()),
					(villageTilemap->GetTileHeight())
				);
			}
		}

		/*
		gueepo::FontSprite* dogica = m_rm->GetFontSprite("dogica-24");
		std::string scoreString = std::to_string(playerScore);
		gueepo::string scoreStringRounded = scoreString.substr(0, scoreString.find(".")+3).c_str();
		float textWidth = dogica->GetWidthOf(scoreStringRounded);
		gueepo::math::vec2 textPosition(300.0f - textWidth, 145.0f);
		gueepo::Renderer::DrawString(dogica, scoreStringRounded, textPosition, 1.0f, gueepo::Color(0.0f, 0.0f, 0.0f, 0.0f));
		*/
	}

}