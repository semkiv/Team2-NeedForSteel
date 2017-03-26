#include "stdafx.h"
#include "LuaManager.h"

CLuaManager CLuaManager::m_instance;

CLuaManager & CLuaManager::GetInstance()
{
    return m_instance;
}

luabridge::lua_State* CLuaManager::GetFileLuaState(std::string fileName)
{
    auto iter = m_luaHash.find(fileName);
    if (iter == m_luaHash.end())
    {
        return m_luaHash.insert(std::make_pair(fileName, CreateLuaState(fileName))).first->second;
    }

    return iter->second;
}

luabridge::lua_State* CLuaManager::CreateLuaState(std::string const& fileName)
{
    luabridge::lua_State* L = luabridge::luaL_newstate();
    luabridge::luaL_openlibs(L);
    luaL_dofile(L, fileName.c_str());

    return L;
}
