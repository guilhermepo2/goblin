#define GUEEPO2D_MAIN
#include <gueepo2d.h>
#include "goblin.h"

static gbln::ResourceManager g_ResourceManager;
static gbln::GameWorld g_GameWorld;
static gueepo::UIManager* g_UI;

struct INITIALIZATION_OPTIONS {
    std::string title;
    int width;
    int height;
};

class GoblinApplication : public gueepo::Application {
public:
    //
	GoblinApplication() : Application("Goblin Game Engine", 640, 360) {}
    GoblinApplication(const INITIALIZATION_OPTIONS& init) : Application(init.title, init.width, init.height) {}
	~GoblinApplication() {}

    void Application_OnInitialize() override;
    void Application_OnDeinitialize() override;
    void Application_OnUpdate(float DeltaTime) override;
    void Application_OnInput(const gueepo::InputState& currentInputState) override;
    void Application_OnRender() override;

private:
    gueepo::OrtographicCamera* m_camera;
};

void GoblinApplication::Application_OnInitialize() {
    m_camera = new gueepo::OrtographicCamera(640, 360);

    g_UI = new gueepo::UIManager(640, 360);
    gbln::Factory::LoadResourceFile(&g_ResourceManager, "./assets/resources.json");
    gbln::Factory::LoadEntity(&g_GameWorld, &g_ResourceManager, "./assets/goblinEntity.json");
    gbln::Factory::LoadUIElement(g_UI, &g_ResourceManager, "./assets/UISample.json");

    g_GameWorld.BeginPlay();
}

void GoblinApplication::Application_OnDeinitialize() {
    g_ResourceManager.ClearResources();
}

void GoblinApplication::Application_OnInput(const gueepo::InputState &currentInputState) {
    g_GameWorld.ProcessInput(currentInputState);
    g_UI->ProcessInput(currentInputState);
}

void GoblinApplication::Application_OnUpdate(float DeltaTime) {
    g_GameWorld.Update(DeltaTime);
    g_UI->Update(DeltaTime);
}

void GoblinApplication::Application_OnRender() {
    gueepo::Renderer::BeginFrame(*m_camera);
    gueepo::Renderer::Clear(0.1f, 0.6f, 0.1f, 1.0f);

    g_GameWorld.Render();
    g_UI->Render();

    gueepo::Renderer::EndFrame();
}

gueepo::Application* gueepo::CreateApplication() {
    gueepo::json configOptions("./assets/init.json");

    if(configOptions.IsValid()) {
        INITIALIZATION_OPTIONS options;
        configOptions.GetString("title", options.title);
        configOptions.GetInt("width", options.width);
        configOptions.GetInt("height", options.height);
        return new GoblinApplication(options);
    } else {
        return new GoblinApplication();
    }
}
