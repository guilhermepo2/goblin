#pragma once
#include "Component.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
};

namespace gbln {
    class LuaComponent : public Component {
    public:
        explicit LuaComponent(const gueepo::string luaScript);
        ~LuaComponent() override;
        void Initialize() override;
        void BeginPlay() override;
        // bool ProcessInput(const gueepo::InputState& CurrentInputState) override;
        void Update(float DeltaTime) override;
        // void Render() override; // is this ever going to be on Lua?
        void Destroy() override;

    private:
        void CallLuaFunction(const gueepo::string& FunctionName);
        bool CheckLua(lua_State* L, int result);
        void BindScriptFunctions(lua_State* L);
        lua_State* m_internalState;
        gueepo::string m_luaAssetPath;
    };
}