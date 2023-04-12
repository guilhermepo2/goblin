#include "LuaComponent.h"

#include "Transform.h"
#include "Entity.h"

gbln::LuaComponent::LuaComponent(const gueepo::string luaScript) {
    m_internalState = luaL_newstate();
    luaL_openlibs(m_internalState);
    BindScriptFunctions(m_internalState);
    m_luaAssetPath = luaScript;

    if(CheckLua(m_internalState, luaL_dofile(m_internalState, luaScript.c_str()))) {
        LOG_INFO("Loaded Script: {0}", luaScript);
    } else {
        LOG_ERROR("Unable to load script: {0}", luaScript);
    }
}

gbln::LuaComponent::~LuaComponent() {}

void gbln::LuaComponent::Initialize() {
    CallLuaFunction("Initialize");
}

void gbln::LuaComponent::BeginPlay() {
    CallLuaFunction("BeginPlay");
}


bool gbln::LuaComponent::ProcessInput(const gueepo::InputState& CurrentInputState) {
    lua_getglobal(m_internalState, "ProcessInput");
    if (lua_isfunction(m_internalState, -1)) {
        lua_pushlightuserdata(m_internalState, owner);
        // todo: build a struct with the inputs... how?
        lua_pcall(m_internalState, 1, 0, 0);
    }

    return false;
}

void gbln::LuaComponent::Update(float DeltaTime) {
    lua_getglobal(m_internalState, "Update");
    if(lua_isfunction(m_internalState, -1)) {
        lua_pushlightuserdata(m_internalState, owner);
        lua_pushnumber(m_internalState, DeltaTime);
        lua_pcall(m_internalState, 2, 0, 0);
    }
}

void gbln::LuaComponent::Destroy() {
    CallLuaFunction("Destroy");
}

void gbln::LuaComponent::CallLuaFunction(const gueepo::string &FunctionName) {
    lua_getglobal(m_internalState, FunctionName.c_str());
    if(lua_isfunction(m_internalState, -1)) {
        // pushing "self" as an argument, this should be done for every function called on Lua from CPP
        lua_pushlightuserdata(m_internalState, owner);
        if(lua_pcall(m_internalState, 1, 0, 0) != 0) {
            LOG_INFO("Couldn't call function {0} on {1}", FunctionName, m_luaAssetPath);
            std::string errormsg = lua_tostring(m_internalState, -1);
            LOG_ERROR(errormsg);
        }
    }
}

bool gbln::LuaComponent::CheckLua(lua_State *L, int result) {
    if(result != LUA_OK) {
        gueepo::string errormsg = lua_tostring(L, -1);
        LOG_ERROR("[LUA] {0}", errormsg);
        return false;
    }
    return true;
}

// ==============================================================================
// ==============================================================================
// LUA API
// ==============================================================================
// ==============================================================================

// void _Log(string)
int lua_Log(lua_State* L);
// void _Move(Entity*, x, y)
int lua_TransformTranslate(lua_State* L);
// void _SetRotation(Entity*, rotation)
int lua_SetRotation(lua_State* L);
// float _GetRotation(Entity*)
int lua_GetCurrentRotation(lua_State* L);

void gbln::LuaComponent::BindScriptFunctions(lua_State *L) {
    lua_register(L, "_Log", lua_Log);
    lua_register(L, "_Move", lua_TransformTranslate);
    lua_register(L, "_SetRotation", lua_SetRotation);
    lua_register(L, "_GetRotation", lua_GetCurrentRotation);
}

int lua_Log(lua_State* L) {
    const char* message = lua_tostring(L, -1);
    LOG_INFO("[lua] {0}", message);
    return 0;
}

int lua_TransformTranslate(lua_State* L) {
    gbln::Entity* owner = static_cast<gbln::Entity*>(lua_touserdata(L, 1));
    float x = lua_tonumber(L, 2);
    float y = lua_tonumber(L, 3);

    gbln::Transform* t = owner->GetComponentOfType<gbln::Transform>();
    if (t != nullptr) {
        t->Translate(gueepo::math::vec2(x, y));
    }

    return 0;
}

int lua_SetRotation(lua_State* L) {
    gbln::Entity* e = static_cast<gbln::Entity*>(lua_touserdata(L, 1));
    float rotation = lua_tonumber(L, 2);
    gbln::Transform* t = e->GetComponentOfType<gbln::Transform>();

    if(t != nullptr) {
        t->rotation = rotation;
    }

    return 0;
}

int lua_GetCurrentRotation(lua_State* L) {
    gbln::Entity* owner = static_cast<gbln::Entity*>(lua_touserdata(L, 1));
    float Rotation = 0.0f;

    gbln::Transform* t = owner->GetComponentOfType<gbln::Transform>();

    if (t != nullptr) {
        Rotation = t->rotation;
    }

    lua_pushnumber(L, Rotation);
    return 1;
}
