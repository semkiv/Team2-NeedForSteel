#pragma once
#include "../Angel/Actors/PhysicsActor.h"
#include "Interfaces.h"
#include <memory>

class CPhysicsUnit:
    public PhysicsActor,
    public std::enable_shared_from_this<CPhysicsUnit>
{    
public:
    virtual ~CPhysicsUnit() = default;
    
    virtual void CustomInitPhysics() override;  
    virtual void OnFree() override;
    virtual void OnCollision(std::shared_ptr<CPhysicsUnit> physicsUnit);
    virtual void EndCollision(std::shared_ptr<CPhysicsUnit> physicsUnit);

    virtual void SetTransform(Vector2 const& pos, float rotation);

    virtual std::shared_ptr<CDamageable> GetDamagable();
    virtual std::shared_ptr<CDamager> GetDamager();
    virtual std::shared_ptr<CTeamMember> GetTeamMember();
    virtual std::shared_ptr<CHealable> GetHealable();
};
