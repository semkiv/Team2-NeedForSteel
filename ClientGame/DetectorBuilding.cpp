#include "stdafx.h"
#include "DetectorBuilding.h"

std::shared_ptr<CTargetDetector> CDetectorBuilding::GetDetector() const
{
    return m_detector;
}

void CDetectorBuilding::SetDetector(std::shared_ptr<CTargetDetector> targetDetector)
{
    AddComponent(m_detector);

    targetDetector->SetParent(shared_from_this());
    m_detector = targetDetector;
}

void CDetectorBuilding::OnFree()
{
    CBuilding::OnFree();

    m_detector.reset();
}

void CDetectorBuilding::SetChildrenPositions(Vector2 const& pos)
{
    SetPosition(pos);

    if (m_detector != nullptr)
    {
        m_detector->SetChildrenPositions(pos);
    }
}
