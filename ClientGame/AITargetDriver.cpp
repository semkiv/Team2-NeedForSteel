#include "stdafx.h"
#include "AITargetDriver.h"
#include "Unit.h"

void CAITargetDriver::OnTargetReached()
{
    auto unit = GetUnit().lock();
    if (unit == nullptr)
    {
        return;
    }

    if (m_useDefaultPath)
    {
        if (++m_currentDefaultPoint == m_defaultPath.size())
        {
            m_currentDefaultPoint = 0;
        }

        m_path = m_pathFinder->FindPath(GetUnit().lock()->GetPosition(), CurrentDefaultPoint());
        if (m_path.size() > 1)
        {
            m_currentPoint = 1;
        }

        MoveToNextPoint();
    }
}

void CAITargetDriver::OnDestinationReached()
{
    CAIDriver::OnDestinationReached();
    MoveToNextPoint();
}

void CAITargetDriver::UpdatePathToTarget()
{
    auto target = GetTarget().lock();
    auto unit = GetUnit().lock();
    
    if (unit == nullptr)
    {
        return;
    }

    if (target == nullptr || target->IsDestroyed())
    {
        SetTarget(nullptr);
        m_useDefaultPath = true;
        m_path = m_pathFinder->FindPath(unit->GetPosition(), CurrentDefaultPoint());        
    }
    else
    {
        m_useDefaultPath = false;
        m_path = m_pathFinder->FindPath(unit->GetPosition(), target->GetPosition());
        m_path.push_back(target->GetPosition());
    }     

    if (m_path.size() > 1)
    {
        m_currentPoint = 1;
    }
    MoveToNextPoint();
}

void CAITargetDriver::DrawPath()
{
    auto unit = GetUnit().lock();
    if (unit == nullptr)
    {
        return;
    }

    else
    {
        for (size_t i = 1; i < m_path.size(); ++i)
        {
            DrawLine(m_path[i - 1], m_path[i]);
        }
    }
}

void CAITargetDriver::SetDefaultPath(std::vector<Vector2> defaultPath)
{
    m_currentDefaultPoint = 0;
    m_defaultPath = defaultPath;
}

void CAITargetDriver::SetPathFinder(std::shared_ptr<IPathFinder> pathFinder)
{
    m_pathFinder = pathFinder;
}

void CAITargetDriver::MoveToNextPoint()
{
    if (m_path.size() == 0)
    {
        return;
    }

    if (m_currentPoint >= m_path.size())
    {
        OnTargetReached();
        return;
    }

    MoveTo(m_path[m_currentPoint++]);
}

Vector2 CAITargetDriver::CurrentDefaultPoint() const
{
    return m_defaultPath[m_currentDefaultPoint];
}

CAIUpdateTarget::CAIUpdateTarget(std::shared_ptr<CTargetEnable> targetEnableUnit, std::shared_ptr<CUnit> detector, std::shared_ptr<IClosestFinder> finder):
    m_targetEnableUnit(targetEnableUnit),
    m_detector(detector),
    m_finder(finder),
    m_radius(0.0f)
{
    m_isEmpty = false;
}

void CAIUpdateTarget::UpdateTarget()
{
    auto targetEnableUnit = m_targetEnableUnit.lock();
    if (targetEnableUnit == nullptr)
    {
        m_isEmpty = true;
        return;
    }

    auto detector = m_detector.lock();
    if (detector == nullptr)
    {
        m_isEmpty = true;
        return;
    }

    auto finder = m_finder.lock();
    if (finder == nullptr)
    {
        m_isEmpty = true;
        return;
    }

    auto target = finder->FindClosestInsideCircle(
        detector->GetPosition(), 
        m_radius, 
        detector->GetTeam()->GetEnemiesMask());

    targetEnableUnit->SetTarget(target);
}

void CAIUpdateTarget::SetRadius(float radius)
{
    m_radius = radius;
}
