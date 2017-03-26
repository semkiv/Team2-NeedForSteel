#include "stdafx.h"
#include "GameUI.h"

CGameUI::CGameUI(size_t number)
{
    m_players.reserve(number);
    Vector2 dimensions = MathUtil::GetWorldDimensions();
    
    m_players.emplace_back(std::make_shared<CPlayerUI>(Vector2{ dimensions.X * -0.5f + 0.5f, dimensions.Y * 0.5f - 0.7f }));
    m_players.emplace_back(std::make_shared<CPlayerUI>(Vector2{ dimensions.X * 0.5f - 3.0f, dimensions.Y * 0.5f - 0.7f }));

    m_shops.emplace_back(std::make_shared<CShopUI>(EScreenSide::LEFT));
    m_shops.emplace_back(std::make_shared<CShopUI>(EScreenSide::RIGHT));

    m_victoryBox = std::make_shared<CVictoryBox>(Vector2{ 0.0f, 0.0f });
}

void CGameUI::UpdateGold(size_t id, size_t newGold)
{
    m_players[id]->SetGold(newGold);
}

std::shared_ptr<CPlayerUI> CGameUI::GetPlayerUI(size_t playerId)
{
    return m_players[playerId];
}

std::shared_ptr<CShopUI> CGameUI::GetShopUI(size_t playerId)
{
    return m_shops[playerId];
}

void CGameUI::ShowVictoryWindow(size_t winnerId)
{
    m_victoryBox->Show(winnerId);
}

void CGameUI::AddSpawnerUI(std::shared_ptr<CSpawnerUI> spawner)
{
    m_spawners.emplace_back(spawner);
}

CGameUI::~CGameUI()
{
    if (m_victoryBox->IsVisible())
    {
        m_victoryBox->Hide();
    }
}
