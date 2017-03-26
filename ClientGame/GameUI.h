#pragma once
#include "Angel.h"
#include "Common.h"
#include "PlayerUI.h"
#include "VictoryBox.h"
#include "ShopUI.h"
#include "SpawnerUI.h"

class CGameUI
{
public:
    CGameUI(size_t number);

    void UpdateGold(size_t id, size_t newGold);
    std::shared_ptr<CPlayerUI> GetPlayerUI(size_t playerId);
    std::shared_ptr<CShopUI> GetShopUI(size_t playerId);
    void ShowVictoryWindow(size_t winnerId);
    void AddSpawnerUI(std::shared_ptr<CSpawnerUI> spawner);

    ~CGameUI();

private:
    std::shared_ptr<CVictoryBox> m_victoryBox;
    std::vector<std::shared_ptr<CPlayerUI>>m_players;
    std::vector<std::shared_ptr<CShopUI>> m_shops;
    std::vector<std::shared_ptr<CSpawnerUI>> m_spawners;

};
