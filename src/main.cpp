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

    int cameraWidth;
    int cameraHeight;

    int uiWidth;
    int uiHeight;
};

class GoblinApplication : public gueepo::Application {
public:
	GoblinApplication() : Application("Goblin Game Engine", 640, 360) {
        m_windowTitle = "Goblin Game Engine";
        m_windowWidth = m_cameraWidth = m_uiWidth = 640;
        m_windowHeight = m_cameraHeight = m_uiHeight = 360;
    }

    GoblinApplication(const INITIALIZATION_OPTIONS& init) : Application(init.title, init.width, init.height) {
        m_windowTitle = init.title.c_str();
        m_windowWidth = init.width;
        m_windowHeight = init.height;

        m_cameraWidth = init.cameraWidth;
        m_cameraHeight = init.cameraHeight;

        m_uiWidth = init.uiWidth;
        m_uiHeight = init.uiHeight;
    }

	~GoblinApplication() {}

    void Application_OnInitialize() override;
    void Application_OnDeinitialize() override;
    void Application_OnUpdate(float DeltaTime) override;
    void Application_OnInput(const gueepo::InputState& currentInputState) override;
    void Application_OnRender() override;

private:
    gueepo::OrtographicCamera* m_camera = nullptr;
    gueepo::string m_windowTitle;
    int m_windowWidth = 0, m_windowHeight = 0;
    int m_cameraWidth = 0, m_cameraHeight = 0;
    int m_uiWidth = 0, m_uiHeight= 0;
};

void GoblinApplication::Application_OnInitialize() {
    m_camera = new gueepo::OrtographicCamera(m_cameraWidth, m_cameraHeight);
    g_UI = new gueepo::UIManager(m_uiWidth, m_uiHeight);
    
    gbln::Factory::LoadResourceFile(&g_ResourceManager, "./assets/resources.json");
    gbln::Factory::LoadEntity(&g_GameWorld, &g_ResourceManager, "./assets/plat_character.json");
    // gbln::Factory::LoadUIElement(g_UI, &g_ResourceManager, "./assets/UISample.json");

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
    gueepo::Renderer::Clear(0.1f, 0.3f, 0.6f, 1.0f);

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

		configOptions.GetInt("width", options.cameraWidth);
		configOptions.GetInt("height", options.cameraHeight);

		configOptions.GetInt("width", options.uiWidth);
		configOptions.GetInt("height", options.uiHeight);
        return new GoblinApplication(options);
    } else {
        return new GoblinApplication();
    }
}
