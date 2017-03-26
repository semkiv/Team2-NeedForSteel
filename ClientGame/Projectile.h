#pragma once

#include "Common.h"
#include "PhysicsUnit.h"
#include "Interfaces.h"
#include "Unit.h"
#include "EventManager.h"

#include <memory>

class CBasicProjectile : public CPhysicsUnit,
    public CDamageable,
    public CDamager,
    public CTeamMember    
{
public:
    CBasicProjectile();
    virtual ~CBasicProjectile() = default;

    virtual void CustomInitPhysics() override;
    virtual void PreCustomInitPhysics() override;
    virtual void CustomDestroy() override;

    Vector2 GetStartingPoint() const;
    void SetStartingPoint(Vector2 const& point);
    std::weak_ptr<CPhysicsUnit> GetOwner() const;
    void SetOwner(std::shared_ptr<CPhysicsUnit> owner);

    virtual void TakeDamage(float damage, EDamageType damageType) override;

    virtual std::shared_ptr<CDamageable> GetDamagable() override;
    virtual std::shared_ptr<CDamager> GetDamager() override;
    virtual std::shared_ptr<CTeamMember> GetTeamMember() override;

    virtual void OnCollision(std::shared_ptr<CPhysicsUnit> physicsUnit) override;

    virtual void OnKilled(std::weak_ptr<CDamageable> victim) override;

private:
    Vector2 m_startingPoint;
    std::weak_ptr<CPhysicsUnit> m_owner;
};

class CProjectile : public CBasicProjectile
{
public:
    float GetFlightDistance() const;
    void SetFlightDistance(float distance);
    virtual void Update(float dt) override;

private:
    float m_flightDistance;
};

class CTargetProjectile : public CBasicProjectile
{
public:
    std::weak_ptr<CPhysicsUnit> GetTarget() const;
    void SetTarget(std::shared_ptr<CPhysicsUnit> target);
    virtual void Update(float dt) override;

private:
    std::weak_ptr<CPhysicsUnit> m_target;
};
