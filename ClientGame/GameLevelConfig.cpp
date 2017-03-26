#include "stdafx.h"
#include "GameLevelConfig.h"
#include "LuaManager.h"

Color LoadColor(luabridge::LuaRef lua);
Vec2i LoadPosition(luabridge::LuaRef lua);
SGameLevelConfig::SHeadQuarterConfig LoadHeadQuarter(luabridge::LuaRef lua);

Color LoadColor(luabridge::LuaRef lua)
{
    Color color;
    int l = lua.length();

    color.R = lua.At(1).cast<float>();
    color.G = lua.At(2).cast<float>();
    color.B = lua.At(3).cast<float>();

    if (lua.length() > 3)
    {
        color.A = lua.At(4).cast<float>();
    }

    return color;
}

Vec2i LoadPosition(luabridge::LuaRef lua)
{
    Vec2i pos{ 0, 0 };

    pos.X = lua.At(1).cast<int>();
    pos.Y = lua.At(2).cast<int>();

    return pos;
}

std::vector<Vec2i> LoadPositions(luabridge::LuaRef lua)
{
    std::vector<Vec2i> positions;

    int len = lua.length();
    for (int i = 1; i <= len; ++i)
    {
        positions.emplace_back(
            LoadPosition(lua.At(i)));
    }

    return positions;
}

SGameLevelConfig::SHeadQuarterConfig LoadHeadQuarter(luabridge::LuaRef lua)
{
    SGameLevelConfig::SHeadQuarterConfig hq;

    if (hq.m_isNull = lua.isNil())
    {
        return hq;
    }

    hq.m_type = lua.At("type").cast<std::string>();
    hq.m_position = LoadPosition(lua.At("position"));

    return hq;
}

SGameLevelConfig::SSpawner LoadSpawner(luabridge::LuaRef lua)
{
    SGameLevelConfig::SSpawner spawner;

    spawner.m_spawnPoint = LoadPosition(lua.At("spawnPoint"));
    spawner.m_spawnAngle = lua.At("spawnAngle").cast<float>();
    spawner.m_spawnTime = lua.At("spawnTime").cast<float>();

    return spawner;
}

SGameLevelConfig::SPlayerConfig LoadPlayer(luabridge::LuaRef lua)
{
    SGameLevelConfig::SPlayerConfig player;

    if (player.m_isNull = lua.isNil())
    {
        return player;
    }

    player.m_tank = lua.At("tank").cast<std::string>();
    player.m_spawner = LoadSpawner(lua.At("spawner"));

    return player;
}

SGameLevelConfig::SMineSpawner LoadMineSpawner(luabridge::LuaRef lua)
{
    SGameLevelConfig::SMineSpawner spawner;

    spawner.m_mine = lua.At("mine").cast<std::string>();
    spawner.m_path = LoadPositions(lua.At("path"));
    spawner.m_spawner = LoadSpawner(lua.At("spawner"));
    spawner.m_detectionRadius = lua.At("detectionRadius").cast<float>();

    return spawner;
}

SGameLevelConfig::STowerConfig LoadTower(luabridge::LuaRef lua)
{
    SGameLevelConfig::STowerConfig tower;

    tower.m_tower = lua.At("tower").cast<std::string>();
    tower.m_position = LoadPosition(lua.At("position"));
    tower.m_angle = lua.At("angle").cast<float>();

    return tower;
}

SGameLevelConfig::SShopConfig LoadShop(luabridge::LuaRef lua)
{
    SGameLevelConfig::SShopConfig shop;

    if (shop.m_isNull = lua.isNil())
    {
        return shop;
    }

    shop.m_shop = lua.At("shop").cast<std::string>();
    shop.m_position = LoadPosition(lua.At("position"));
    shop.m_detectionRadius = lua.At("detectionRadius").cast<float>();

    return shop;
}

std::shared_ptr<SGameLevelConfig::STeamConfig> LoadTeam(luabridge::LuaRef lua)
{
    std::shared_ptr<SGameLevelConfig::STeamConfig> team = std::make_shared<SGameLevelConfig::STeamConfig>();

    team->m_team = StringToTeam(lua.At("team").cast<std::string>());
    team->m_color = LoadColor(lua.At("color"));
    team->m_headQuarter = LoadHeadQuarter(lua.At("headQuarter"));

    auto luaMineSpawners = lua.At("mineSpawners");
    
    if (!luaMineSpawners.isNil())
    {
        auto mineSpawnersLen = luaMineSpawners.length();

        for (int i = 1; i <= mineSpawnersLen; ++i)
        {
            team->m_mineSpawners.emplace_back(
                LoadMineSpawner(luaMineSpawners.At(i)));
        }
    }

    auto luaTowers = lua.At("towers");    

    if (!luaTowers.isNil())
    {
        auto towersLen = luaTowers.length();
        for (int i = 1; i <= towersLen; ++i)
        {
            team->m_towers.emplace_back(
                LoadTower(luaTowers.At(i)));
        }
    }

    team->m_shop = LoadShop(lua.At("shop"));
    team->m_player = LoadPlayer(lua.At("player"));

    return team;
}

std::shared_ptr<SGameLevelConfig> LoadGameLevelConfig(std::string const& fileName)
{
    using namespace luabridge;

    lua_State* L = theLuaState(fileName);

    std::shared_ptr<SGameLevelConfig> levelConfig = std::make_shared<SGameLevelConfig>();
        
    luabridge::LuaRef teams = getGlobal(L, "teams");
    int teamsCnt = teams.length();
    for (int i = 1; i <= teamsCnt; ++i)
    {
        levelConfig->m_teams.push_back(
            LoadTeam(teams.At(i)));
    }

    levelConfig->m_towerSize = LoadPosition(getGlobal(L, "towerSize"));
    levelConfig->m_headQuarterSize = LoadPosition(getGlobal(L, "headQuarterSize"));
    levelConfig->m_tiledMapFile = getGlobal(L, "tiledMapFile").cast<std::string>();
    levelConfig->m_income = getGlobal(L, "income").cast<float>();

    return levelConfig;
}
