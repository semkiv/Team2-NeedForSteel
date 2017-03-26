#include "stdafx.h"
#include "TargetDetection.h"

void CTargetDetector::OnCollision(std::shared_ptr<ValueType> physicsUnit)
{
    if (Check(physicsUnit))
    {
        m_targets.insert(physicsUnit);
    }
}

void CTargetDetector::EndCollision(std::shared_ptr<ValueType> physicsUnit)
{
    if (Check(physicsUnit))
    {
        m_targets.erase(physicsUnit);
    }
}

std::shared_ptr<CTeamMember> CTargetDetector::GetTeamMember()
{
    if (IsDestroyed())
    {
        return nullptr;
    }

    assert(GetParent().lock() != nullptr);

    return GetParent().lock()->GetTeamMember();
}

bool CTargetDetector::IsTargretValid(std::shared_ptr<ValueType> target) const
{
    return m_targets.find(target) != m_targets.end();
}

void CTargetDetector::CustomDestroy()
{    
    m_targets.clear();
    CPhysicsUnit::CustomDestroy();
}

bool CTargetDetector::Check(std::shared_ptr<CTargetDetector::ValueType> physicsUnit)
{
    if (physicsUnit->IsBitTagged(static_cast<size_t>(ETag::DETECTABLE)))
    {
        auto t1 = physicsUnit->GetTeamMember();
        auto t2 = GetTeamMember();

        if (!t1 || !t2)
        {
            false;
        }

        return true;
    }

    return false;
}

CTargetDetector::CTargetDetector()
{
    TagBit(static_cast<size_t>(ETag::DETECTOR));
}

CTargetDetector::~CTargetDetector() = default;

CTargetDetector::ContainerType& CTargetDetector::GetTargets()
{
    for (auto it{ m_targets.cbegin() }; it != m_targets.cend();)
    {
        auto ptr = it->lock();
        if (ptr && ptr->IsDestroyed())
        {
            it = m_targets.erase(it);
        }
        else
        {
            ++it;
        }
    }

    return m_targets;
}
