#include "stdafx.h"
#include "SpawnerUI.h"

CSpawnerUI::CSpawnerUI(Vector2 position, Color color, Vector2 size):
    m_circle(std::make_shared<Actor>()),
    m_size(size)
{
    m_circle->SetDrawShape(ADS_Circle);
    m_circle->SetPosition(position);
    m_circle->SetSize(0.0f, 0.0f);
    m_circle->SetColor(color);
    theWorld.Add(m_circle.get());
}

void CSpawnerUI::OnCooldownChanged(float currentCooldown, float cooldown)
{
    m_circle->SetSize(m_size*(currentCooldown / cooldown));
}

CSpawnerUI::~CSpawnerUI()
{
    theWorld.Remove(m_circle.get());
}
