#include "stdafx.h"
#include "UpgradeConfig.h"
#include "LuaManager.h"

EUpgrade StringToUpgrade(std::string str)
{
    EUpgrade upgrade = EUpgrade::HP;

    if (str == "HP")
    {
        upgrade = EUpgrade::HP;
    }
    else if (str == "DAMAGE")
    {
        upgrade = EUpgrade::DAMAGE;
    }
    else if (str == "COOLDOWN")
    {
        upgrade = EUpgrade::COOLDOWN;
    }

    return upgrade;
}

SUpgradesConfig::SUpgrade LoadUpgrade(luabridge::LuaRef lua)
{
    SUpgradesConfig::SUpgrade upgrade{};

    upgrade.m_price = static_cast<size_t>(lua.At("price").cast<int>());
    upgrade.m_value = lua.At("value").cast<float>();

    return upgrade;
}

SUpgradesConfig::SUpgradeSequence LoadUpgradeSequence(luabridge::LuaRef lua)
{
    SUpgradesConfig::SUpgradeSequence upgrade{};

    upgrade.m_upgrade = StringToUpgrade(lua.At("upgrade").cast<std::string>());

    auto luaSequence = lua.At("sequence");
    int len = luaSequence.length();

    for (int i = 1; i <= len; ++i)
    {
        upgrade.m_sequence.emplace_back(
            LoadUpgrade(luaSequence.At(i)));
    }

    return upgrade;
}

std::shared_ptr<SUpgradesConfig> LoadUpgradesConfig(std::string const& file)
{    
    using namespace luabridge;

    lua_State* L = theLuaState(file);

    std::shared_ptr<SUpgradesConfig> config = std::make_shared<SUpgradesConfig>();

    auto luaUpgrades = luabridge::getGlobal(L, "upgrades");
    int len = luaUpgrades.length();

    for (int i = 1; i <= len; ++i)
    {
        config->m_upgrades.emplace_back(
            LoadUpgradeSequence(luaUpgrades.At(i)));
    }

    return config;
}
