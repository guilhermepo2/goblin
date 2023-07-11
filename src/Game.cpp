#include "Game.h"

static float playerScore = 0;

static float timeElapsed = 0.0f;
static gbln::Entity* playerEntity = nullptr;
static gbln::Entity* yetiEntity = nullptr;
// // //

namespace gbln {
    
	Game::Game() {}
	Game::~Game() {}

	void Game::Begin() {
		playerEntity = m_gw->GetEntityByName("skier");
		yetiEntity = m_gw->GetEntityByName("yeti");
	}

	void Game::Update(float deltaTime) {
		playerScore += deltaTime;
		timeElapsed += deltaTime;
		float currentCos = gueepo::math::cos(timeElapsed);

		if (yetiEntity != nullptr && playerEntity != nullptr) {
			Transform* yetiTransform = yetiEntity->GetComponentOfType<Transform>();
			Transform* playerTransform = playerEntity->GetComponentOfType<Transform>();
			yetiTransform->position.x = playerTransform->position.x;
			yetiTransform->position.y = playerTransform->position.y + 65 + (currentCos * 7);
		}

	}

	void Game::LoadResources(ResourceManager* rm, GameWorld* gw) {
		m_rm = rm;
		m_gw = gw;
		gbln::Factory::LoadResourceFile(rm, "./assets/resources.json");
		gueepo::Texture* tinySkiTilemap = rm->GetTexture("kenney_tiny_ski");
		gueepo::Tilemap* skiTilemap = gbln::Factory::CreateTilemapFromFile(tinySkiTilemap, "./assets/gameplay_map.json");


		gbln::Entity* map = gw->AddEntity("main tilemap");
		map->AddComponent<gbln::Transform>(
			gueepo::math::vec2(-335.0f, -197.0f),
			0.0f,
			gueepo::math::vec2(2.25f, 2.25f)
		);
		map->AddComponent<gbln::TilemapComponent>(skiTilemap);
		
		gbln::Factory::LoadEntity(gw, rm, "./assets/ski_char_entity.json");
		gbln::Factory::LoadEntity(gw, rm, "./assets/yeti_entity.json");
	}

	void Game::Render() {
		gueepo::FontSprite* dogica = m_rm->GetFontSprite("dogica-24");
		std::string scoreString = std::to_string(playerScore);
		gueepo::string scoreStringRounded = scoreString.substr(0, scoreString.find(".")+3).c_str();
		float textWidth = dogica->GetWidthOf(scoreStringRounded);
		gueepo::math::vec2 textPosition(300.0f - textWidth, 145.0f);
		gueepo::Renderer::DrawString(dogica, scoreStringRounded, textPosition, 1.0f, gueepo::Color(0.0f, 0.0f, 0.0f, 0.0f));
	}

}