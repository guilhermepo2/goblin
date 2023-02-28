#define GUEEPO2D_MAIN
#include <gueepo2d.h>

#include "ResourceManager.h"
static gbln::ResourceManager g_ResourceManager;

class GoblinApplication : public gueepo::Application {
public:
	GoblinApplication() : Application("Goblin Game Engine", 640, 360) {}
	~GoblinApplication() {}

    void Application_OnInitialize() override;
    void Application_OnRender() override;

private:
    gueepo::OrtographicCamera* m_camera;
};

void GoblinApplication::Application_OnInitialize() {
    m_camera = new gueepo::OrtographicCamera(640, 360);
    g_ResourceManager.LoadResource("./assets/resources.json");
}

void GoblinApplication::Application_OnRender() {
    gueepo::Texture* archer = g_ResourceManager.GetTexture("the_goblin_archer");
    gueepo::Texture* barbarian = g_ResourceManager.GetTexture("the_goblin_barbarian");
    gueepo::Texture* king = g_ResourceManager.GetTexture("the_goblin_king");
    gueepo::Texture* warrior = g_ResourceManager.GetTexture("the_goblin_warrior");
    gueepo::Texture* wizard = g_ResourceManager.GetTexture("the_goblin_wizard");

    gueepo::Texture* lofi_goblins = g_ResourceManager.GetTexture("lofi_goblins");

    gueepo::Renderer::BeginFrame(*m_camera);
    gueepo::Renderer::Clear(0.5f, 0.1f, 0.1f, 1.0f);

    gueepo::Renderer::Draw(archer   , -128, 128, 64, 64);
    gueepo::Renderer::Draw(barbarian, -64, 128, 64, 64);
    gueepo::Renderer::Draw(king     , 0, 128, 64, 64);
    gueepo::Renderer::Draw(warrior  , 64, 128, 64, 64);
    gueepo::Renderer::Draw(wizard   , 128, 128, 64, 64);
    gueepo::Renderer::Draw(lofi_goblins, 0, 48, 48 * 4, 32);

    gueepo::Renderer::EndFrame();
}

gueepo::Application* gueepo::CreateApplication() {
	return new GoblinApplication();
}
