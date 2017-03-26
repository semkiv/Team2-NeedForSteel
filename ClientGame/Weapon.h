#pragma once

#include "../Angel/Actors/Actor.h"

class CRangeWeapon : public Actor
{

public:
    virtual ~CRangeWeapon() = default;

	float GetCoolDown() const;
	void SetCoolDown(float time);
    void Reload();

    virtual void CustomReset() override;

	virtual void Shoot() = 0;
	bool IsReloaded();
	virtual void Update(float dt) override;

protected:
	float m_currentCoolDown;
	float m_coolDown;
};
