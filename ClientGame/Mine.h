#pragma once
#include "Unit.h"

class CMine : public CControlableUnit
{
public:
    virtual void Update(float dt) override;

    virtual void CustomDestroy() override;
    virtual void TakeDamage(float damage, EDamageType damageType) override;

    void Boom();
    void SetExplosionDamage(float explosionDamage);
    void SetDamageRadius(float damageRadius);
 
private:
    float m_damageRadius;
    float m_explosionDamage;
};
