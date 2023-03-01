#define GUEEPO2D_MAIN
#include <gueepo2d.h>
#include "goblin.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
};

static gbln::ResourceManager g_ResourceManager;
static gbln::GameWorld g_GameWorld;
static lua_State* g_luaState;

class GoblinApplication : public gueepo::Application {
public:
	GoblinApplication() : Application("Goblin Game Engine", 640, 360) {}
	~GoblinApplication() {}

    void Application_OnInitialize() override;
    void Application_OnUpdate(float DeltaTime) override;
    void Application_OnInput(const gueepo::InputState& currentInputState) override;
    void Application_OnRender() override;

private:
    gueepo::OrtographicCamera* m_camera;
};

int lua_Log(lua_State* L) {
    const char* message = lua_tostring(L, -1);
    LOG_INFO("[lua] {0}", message);
    return 0;
}

void GoblinApplication::Application_OnInitialize() {
    m_camera = new gueepo::OrtographicCamera(640, 360);
    g_luaState = luaL_newstate();
    luaL_openlibs(g_luaState);
    lua_register(g_luaState, "_Log", lua_Log);

    int r = luaL_dofile(g_luaState, "./assets/test.lua");
    if(r != LUA_OK) {
        std::string errormsg = lua_tostring(g_luaState, -1);
        LOG_ERROR("lua error: {0}", errormsg);
    } else {
        lua_getglobal(g_luaState, "TestFunction");
        if(lua_isfunction(g_luaState, -1)) {
            lua_pcall(g_luaState, 0, 0, 0);
        }
    }

    g_ResourceManager.LoadResource("./assets/resources.json");

    gbln::Entity* goblinKing = g_GameWorld.AddEntity("Goblin King");
    goblinKing->AddComponent<gbln::Transform>(gueepo::math::vec2(0.0f, 0.0f), .0f, gueepo::math::vec2(3.0f, 3.0f));
    goblinKing->AddComponent<gbln::Sprite>(g_ResourceManager.GetTexture("the_goblin_king"));

    g_GameWorld.BeginPlay();
}

void GoblinApplication::Application_OnInput(const gueepo::InputState &currentInputState) {
    g_GameWorld.ProcessInput(currentInputState);
}

void GoblinApplication::Application_OnUpdate(float DeltaTime) {
    gbln::Entity* goblinKing = g_GameWorld.GetEntityByName("Goblin King");

    if(goblinKing != nullptr) {
        goblinKing->GetComponentOfType<gbln::Transform>()->rotation += 1.0f * DeltaTime;
    }

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
