#define GUEEPO2D_MAIN
#include <gueepo2d.h>
#include "goblin.h"

static gbln::ResourceManager g_ResourceManager;
static gbln::GameWorld g_GameWorld;

class GoblinApplication : public gueepo::Application {
public:
    //
	GoblinApplication() : Application("Goblin Game Engine", 640, 360) {}
	~GoblinApplication() {}

    void Application_OnInitialize() override;
    void Application_OnUpdate(float DeltaTime) override;
    void Application_OnInput(const gueepo::InputState& currentInputState) override;
    void Application_OnRender() override;

private:
    gueepo::OrtographicCamera* m_camera;
};

void GoblinApplication::Application_OnInitialize() {
    m_camera = new gueepo::OrtographicCamera(640, 360);

    g_ResourceManager.LoadResource("./assets/resources.json");

    gbln::Entity* goblinKing = g_GameWorld.AddEntity("Goblin King");
    goblinKing->AddComponent<gbln::Transform>(gueepo::math::vec2(0.0f, 0.0f), .0f, gueepo::math::vec2(3.0f, 3.0f));
    goblinKing->AddComponent<gbln::Sprite>(g_ResourceManager.GetTexture("the_goblin_king"));
    goblinKing->AddComponent<gbln::LuaComponent>("./assets/test.lua");

    g_GameWorld.BeginPlay();
}

void GoblinApplication::Application_OnInput(const gueepo::InputState &currentInputState) {
    g_GameWorld.ProcessInput(currentInputState);
}

void GoblinApplication::Application_OnUpdate(float DeltaTime) {
    g_GameWorld.Update(DeltaTime);
}

void GoblinApplication::Application_OnRender() {
    gueepo::Renderer::BeginFrame(*m_camera);
    gueepo::Renderer::Clear(0.5f, 0.1f, 0.1f, 1.0f);
    g_GameWorld.Render();
    gueepo::Renderer::EndFrame();
}

gueepo::Application* gueepo::CreateApplication() {
	return new GoblinApplication();
}
