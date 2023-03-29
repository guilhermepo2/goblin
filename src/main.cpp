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

    // #todo: all this should move to a "Factory" class
	g_ResourceManager.LoadResource("./assets/resources.json");
	g_ResourceManager.LoadTextureRegions("./assets/planes.json");
	g_ResourceManager.AddFontSpriteFromPath("dogica-8", 8, "./assets/dogica/TTF/dogicapixelbold.ttf");
	g_ResourceManager.AddFontSpriteFromPath("dogica-24", 24, "./assets/dogica/TTF/dogicapixelbold.ttf");

    g_UI = new gueepo::UIManager(640, 360);

    gueepo::FontSprite* dogicaSmall = g_ResourceManager.GetFontSprite("dogica-8");
    gueepo::FontSprite* dogica = g_ResourceManager.GetFontSprite("dogica-24");

    // #todo: all these labels should go into a UI json file on the "Factory" class
    gueepo::Label* copywrightText = new gueepo::Label("(c) gueepo", dogicaSmall);
    copywrightText->SetPosition(gueepo::math::vec2(0.0f, -165.0f));
    copywrightText->SetAlignment(gueepo::ALIGNMENT::CENTER);
    g_UI->Push(copywrightText);

    gueepo::Label* goblinGameEngine = new gueepo::Label("The\nGoblin\nGame Engine", dogica);
    goblinGameEngine->SetPosition(gueepo::math::vec2(-20.0f, 30.0f));
    g_UI->Push(goblinGameEngine);

    // #todo: the creation of entities should move to a .json file on the "Factory" class
    gbln::Entity* goblinKing = g_GameWorld.AddEntity("Goblin King");
    goblinKing->AddComponent<gbln::Transform>(gueepo::math::vec2(-100.0f, 0.0f), .0f, gueepo::math::vec2(-6.0f, 6.0f));
    goblinKing->AddComponent<gbln::Sprite>(g_ResourceManager.GetTexture("the_goblin_king"));
    goblinKing->AddComponent<gbln::LuaComponent>("./assets/test.lua");

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
