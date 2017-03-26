#pragma once

#include <string>
#include <unordered_map>

#include "LuaBridge.h"
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#define theLua CLuaManager::GetInstance()
#define theLuaState CLuaManager::GetInstance().GetFileLuaState

class CLuaManager
{
public:
    static CLuaManager& GetInstance();

    luabridge::lua_State* GetFileLuaState(std::string fileName);

private:
    static CLuaManager m_instance;

private:
    CLuaManager() = default;
    std::unordered_map<std::string, luabridge::lua_State*> m_luaHash;

private:
    luabridge::lua_State* CreateLuaState(std::string const& fileName);
};
