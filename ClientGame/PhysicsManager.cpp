#include "stdafx.h"
#include "PhysicsManager.h"

void CPhysicsManager::AddPhysicsUnit(std::shared_ptr<CPhysicsUnit> unit)
{
    m_units.push_back(unit);
}

std::vector<std::shared_ptr<CPhysicsUnit>> CPhysicsManager::FindAllInsideCircle(Vector2 position, float radius)
{
    std::vector<std::shared_ptr<CPhysicsUnit>> result;
    radius *= radius;

    ForeachValidWeak(m_units, [&radius, &result, &position](std::shared_ptr<CPhysicsUnit> unit)
    {
        float d = (position - unit->GetPosition()).LengthSquared();

        if (d <= radius)
        {
            result.push_back(unit);
        }
    });

    return result;
}

std::shared_ptr<CPhysicsUnit> CPhysicsManager::FindClosestInsideCircle(Vector2 position, float radius, CTeam::MaskType mask)
{
    std::shared_ptr<CPhysicsUnit> result;
    radius *= radius;
    float cur = radius;

    ForeachValidWeak(m_units, [&result, &position, &cur, mask](std::shared_ptr<CPhysicsUnit> unit)
    {
        float d = (position - unit->GetPosition()).LengthSquared();

        auto teamMamber = unit->GetTeamMember();
        if (!unit->IsBitTagged(static_cast<size_t>(ETag::TARGETABLE)) || teamMamber == nullptr)
        {
            return;
        }
        
        CTeam::MaskType team = static_cast<CTeam::MaskType>(static_cast<CTeam::MaskType>(1) << static_cast<size_t>(teamMamber->GetTeam()->GetTeamType()));
        if (d <= cur && (team & mask))
        {
            result = unit;
        }
    });

    return result;
}
