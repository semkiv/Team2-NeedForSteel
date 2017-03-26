#include "stdafx.h"
#include "HeadQuarter.h"

void CHeadQuarter::Update(float dt)
{
    CDetectorBuilding::Update(dt);

    decltype(auto) targetsContainer = m_detector->GetTargets();
    for (auto it{ targetsContainer.cbegin() }; it != targetsContainer.cend(); ++it)
    {
        auto ptr = it->lock();
        if (ptr && !GetTeamMember()->IsEnemy(ptr->GetTeamMember()) && ptr->GetHealable())
        {
            ptr->GetHealable()->Heal(m_healingPower * dt);
        }
    }
}

void CHeadQuarter::SetHealingPower(float healingPower)
{
    m_healingPower = healingPower;
}
