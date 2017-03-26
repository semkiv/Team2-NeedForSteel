#pragma once
#include "../Angel/Actors/Actor.h"
#include "Shop.h"
#include <vector>

enum class EScreenSide
{
    LEFT,
    RIGHT
};

enum class ENavigationCommand
{
    UP,
    DOWN,
    UPGRADE
};

class CShopUI
{
public:
    CShopUI(EScreenSide side = EScreenSide::LEFT);
    ~CShopUI();
    void Show();
    void Hide();
    size_t AcceptCommand(ENavigationCommand command);
    size_t CurrentIcon() const;
    bool IsActive() const;

    void SetUpgradePrice(std::shared_ptr<IUpgradePrice> shop);

private:
    void UpdatePrice();
    struct  SIconUI: public Actor
    {
        //Actor m_icon;
        void HighLight(bool state);
        void Upgrade();
        size_t currentLevel = 0;
    };
    EScreenSide m_side;
    bool m_state;
    size_t m_currentIcon;
    Actor m_background;
    std::vector<SIconUI> m_icons;
    float m_moveToTime;
    float m_iconSize;
    std::weak_ptr<IUpgradePrice> m_upgradePrice;
    TextActor m_currentPrice;
};
