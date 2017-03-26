#include "stdafx.h"
#include "PlayerUI.h"
#include "UI/UserInterface.h"

CPlayerUI::CPlayerUI(Vector2 position) :
    m_gold(std::make_shared<TextActor>())
{
    theWorld.Add(m_gold.get(),static_cast<int>(ELayer::UI));
    m_gold->SetSize(5.0f, 5.0f);
    m_gold->SetPosition(position);
    m_gold->SetFont(theUI.GetDefaultFont());
    m_gold->SetColor(theUI.GetDefaultColor());
    SetGold(0);
}

void CPlayerUI::OnGoldChanged(size_t gold)
{
    SetGold(gold);
}

void CPlayerUI::SetGold(size_t gold)
{
    m_gold->SetDisplayString("Gold: " + std::to_string(gold));
}

CPlayerUI::~CPlayerUI()
{
    theWorld.Remove(m_gold.get());
}
