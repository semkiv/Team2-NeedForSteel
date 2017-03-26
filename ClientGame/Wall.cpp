#include "stdafx.h"
#include "Wall.h"

void CWall::OnCollision(std::shared_ptr<CPhysicsUnit> physicsUnit)
{
    auto damageable = physicsUnit->GetDamagable();
    if (damageable != nullptr)
    {
        damageable->TakeDamage(0, EDamageType::COLLISION);
    }
}
