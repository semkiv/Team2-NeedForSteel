#pragma once

#include <vector>
#include <string>
#include <memory> 

enum class EUpgrade
{
    DAMAGE,
    HP,
    COOLDOWN
};

struct SUpgradesConfig
{
    struct SUpgrade
    {
        float m_value;
        size_t m_price;
    };

    struct SUpgradeSequence
    {
        EUpgrade m_upgrade;
        std::vector<SUpgrade> m_sequence;
    };
    
    std::vector<SUpgradeSequence> m_upgrades;

};

EUpgrade StringToUpgrade(std::string str);
std::shared_ptr<SUpgradesConfig> LoadUpgradesConfig(std::string const& file);
