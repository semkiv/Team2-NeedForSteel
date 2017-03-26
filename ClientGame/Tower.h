#pragma once

#include "Turret.h"
#include "DetectorBuilding.h"
#include <memory>

class CTower: public CDetectorBuilding
{
public:
    std::shared_ptr<CTargetTurret> GetTurret() const;
    void SetTurret(std::shared_ptr<CTargetTurret> turret);
    
    virtual void Update(float dt) override;
    virtual void OnFree() override;

    virtual void SetChildrenPositions(Vector2 const& pos) override;
    virtual void SetChildrenColors(Color const& color) override;
    virtual void SetChildrenRotations(float rotation) override;

    virtual ~CTower() = default;

private:
    std::shared_ptr<CTargetTurret> m_turret;
};
