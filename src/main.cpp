#define GUEEPO2D_MAIN
#include <gueepo2d.h>
#include "goblin.h"

static gbln::ResourceManager g_ResourceManager;
static gbln::GameWorld g_GameWorld;
static gueepo::UIManager* g_UI;
static gueepo::FontSprite* g_dogica = nullptr;
static gueepo::FontSprite* g_dogicaSmall = nullptr;

static gueepo::SpriteAnimation planeAnimation;

class GoblinApplication : public gueepo::Application {
public:
    //
	GoblinApplication() : Application("Goblin Game Engine", 640, 360) {}
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
    //
    m_camera = new gueepo::OrtographicCamera(640, 360);

    gueepo::Font* dogicaPixelFontFile = gueepo::Font::CreateNewFont("./assets/dogica/TTF/dogicapixelbold.ttf");
    if(dogicaPixelFontFile != nullptr) {
        g_dogica = new gueepo::FontSprite(dogicaPixelFontFile, 24);
        g_dogica->SetLineGap(27.0f);

        g_dogicaSmall = new gueepo::FontSprite(dogicaPixelFontFile, 8);
        g_dogicaSmall->SetLineGap(8.0f);
    }

    g_UI = new gueepo::UIManager(640, 360);

    gueepo::Label* copywrightText = new gueepo::Label("(c) gueepo", g_dogicaSmall);
    copywrightText->SetPosition(gueepo::math::vec2(0.0f, -165.0f));
    copywrightText->SetAlignment(gueepo::ALIGNMENT::CENTER);
    g_UI->Push(copywrightText);

    gueepo::Label* goblinGameEngine = new gueepo::Label("The\nGoblin\nGame Engine", g_dogica);
    goblinGameEngine->SetPosition(gueepo::math::vec2(-20.0f, 30.0f));
    g_UI->Push(goblinGameEngine);

    //
    g_ResourceManager.LoadResource("./assets/resources.json");
    g_ResourceManager.LoadTextureRegions("./assets/planes.json");

    gbln::Entity* goblinKing = g_GameWorld.AddEntity("Goblin King");
    goblinKing->AddComponent<gbln::Transform>(gueepo::math::vec2(-100.0f, 0.0f), .0f, gueepo::math::vec2(-6.0f, 6.0f));
    goblinKing->AddComponent<gbln::Sprite>(g_ResourceManager.GetTexture("the_goblin_king"));
    goblinKing->AddComponent<gbln::LuaComponent>("./assets/test.lua");

    gbln::Entity* plane = g_GameWorld.AddEntity("theplane");
    plane->AddComponent<gbln::Transform>(gueepo::math::vec2(0.0f, 0.0f), .0f, gueepo::math::vec2(1.0f, 1.0f));
    plane->AddComponent<gbln::Sprite>();
    gbln::SpriteAnimation* planeAnim = plane->AddComponent<gbln::SpriteAnimation>();
    planeAnim->AddAnimationFrame(g_ResourceManager.GetTextureRegion("planeBlue1"), 0.1f);
    planeAnim->AddAnimationFrame(g_ResourceManager.GetTextureRegion("planeBlue2"), 0.1f);
    planeAnim->AddAnimationFrame(g_ResourceManager.GetTextureRegion("planeBlue3"), 0.1f);

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
    gueepo::SpriteAnimation_Update(planeAnimation, DeltaTime);
}

void GoblinApplication::Application_OnRender() {
    gueepo::Renderer::BeginFrame(*m_camera);
    gueepo::Renderer::Clear(0.1f, 0.6f, 0.1f, 1.0f);

    g_GameWorld.Render();
    // g_UI->Render();
    gueepo::Renderer::EndFrame();
}

gueepo::Application* gueepo::CreateApplication() {
	return new GoblinApplication();
}
