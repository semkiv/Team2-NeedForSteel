#pragma once
#include "DetectorBuilding.h"

class CHeadQuarter :
    public CDetectorBuilding
{
public:
    virtual void Update(float dt) override;
    void SetHealingPower(float healingPower);
private:
    float m_healingPower;
};
