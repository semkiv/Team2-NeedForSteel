#pragma once
#include "Building.h"
#include "TargetDetection.h"

class CDetectorBuilding :
    public CBuilding
{
public:
    std::shared_ptr<CTargetDetector> GetDetector() const;
    void SetDetector(std::shared_ptr<CTargetDetector> targetDetector);
    virtual void OnFree() override;
    virtual void SetChildrenPositions(Vector2 const& pos) override;

protected:
    std::shared_ptr<CTargetDetector> m_detector;
};
