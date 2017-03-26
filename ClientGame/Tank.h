#pragma once

#include "Unit.h"
#include "Turret.h"
#include <memory>

class CTank : public CControlableUnit
{

public:
    virtual ~CTank() = default;

	std::shared_ptr<CTurret> GetTurret() const;
    void SetTurret(std::shared_ptr<CTurret> turret);
    float GetFeedBack() const;
    void SetFeedBack(float feedback);
    
    void HandleTurretRotationSignal(float signal);
	void HandleShootSignal(float signal);

	virtual void Update(float dt) override;
    virtual void OnFree() override;
    virtual void CustomReset() override;

    virtual void SetChildrenPositions(Vector2 const& pos) override;
    virtual void SetChildrenColors(Color const& color) override;
    virtual void SetChildrenRotations(float rotation) override;
    virtual void SetTransform(Vector2 const& pos, float rotation) override;

private:
	std::shared_ptr<CTurret> m_turret;
    float m_feedback;
};
