#include "stdafx.h"
#include "Shop.h"

CShop::CShop(std::string const& configFile)
{
    m_config = LoadUpgradesConfig(configFile);

    m_maxUp.resize(m_config->m_upgrades.size());

    for (size_t i = 0; i < m_config->m_upgrades.size(); ++i)
    {
        m_maxUp[i] = m_config->m_upgrades[i].m_sequence.size();
    }
}

size_t CShop::UpgradesCount() const
{
    return m_config->m_upgrades.size();
}

bool CShop::CanUpgrade(EUpgrade upgrade, int level) const
{
    size_t index = static_cast<size_t>(upgrade);
    return level < m_maxUp[index];
}

size_t CShop::UpgradePrice(EUpgrade upgrade, int level) const
{
    size_t index = static_cast<size_t>(upgrade);
    if (level == m_maxUp[index])
    {
        return 0;
    }
    
    return m_config->m_upgrades[index].m_sequence[level].m_price;
}

float CShop::Upgrade(EUpgrade upgrade, int level) const
{
    assert(CanUpgrade(upgrade, level));

    size_t index = static_cast<size_t>(upgrade);
    return m_config->m_upgrades[index].m_sequence[level].m_value;
}
