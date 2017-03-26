#pragma once

#include "UpgradeConfig.h"
#include <string>
#include <memory>

class IUpgradePrice
{
public:
    virtual size_t UpgradePrice(EUpgrade upgrade, int level) const = 0;
};

class CShop:
    public IUpgradePrice
{
public:
    CShop(std::string const& configFile);

    size_t UpgradesCount() const;
    bool CanUpgrade(EUpgrade upgrade, int level) const;
    virtual size_t UpgradePrice(EUpgrade upgrade, int level) const override;
    float CShop::Upgrade(EUpgrade upgrade, int level) const;

private:
    std::shared_ptr<SUpgradesConfig> m_config;
    std::vector<int> m_maxUp;
};
