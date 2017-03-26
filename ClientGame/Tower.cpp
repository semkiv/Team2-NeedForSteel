#include "stdafx.h"
#include "Tower.h"
#include "Team.h"

std::shared_ptr<CTargetTurret> CTower::GetTurret() const
{
	return m_turret;
}

void CTower::SetTurret(std::shared_ptr<CTargetTurret> turret)
{
    AddComponent(turret);

    turret->SetParent(shared_from_this());
    m_turret = turret;
}

void CTower::Update(float dt)
{
    CUnit::Update(dt);

    auto target = m_turret->GetTarget().lock();
    if (target == nullptr || target->IsDestroyed() || !m_detector->IsTargretValid(target))
    {
        decltype(auto) targetsContainer = m_detector->GetTargets();
        std::shared_ptr<decltype(m_turret)::element_type::TargetType> ptr;

        for (auto it{ targetsContainer.begin() }; it != targetsContainer.end(); ++it)
        {
            ptr = it->lock();
            if (ptr != nullptr && !ptr->IsDestroyed() && GetTeamMember()->IsEnemy(ptr->GetTeamMember()))
            {
                break;
            }
            ptr.reset();
        }
        m_turret->SetTarget(ptr);
    }
}

void CTower::OnFree()
{
    m_turret.reset();
    CDetectorBuilding::OnFree();
}

void CTower::SetChildrenPositions(Vector2 const& pos)
{
    SetPosition(pos);

    if (m_turret != nullptr)
    {
        m_turret->SetChildrenPositions(pos);
    }

    if (m_detector != nullptr)
    {
        m_detector->SetChildrenPositions(pos);
    }
}

void CTower::SetChildrenColors(Color const& color)
{
    SetColor(color);

    if (m_turret != nullptr)
    {
        m_turret->SetChildrenColors(color);
    }
}

void CTower::SetChildrenRotations(float rotation)
{
    SetRotation(rotation);

    if (m_turret != nullptr)
    {
        m_turret->SetChildrenRotations(rotation);
    }

    if (m_detector != nullptr)
    {
        m_detector->SetChildrenRotations(rotation);
    }
}
