#pragma once

#include "Angel.h"
#include "Common.h"
#include "Interfaces.h"
#include "PhysicsUnit.h"
#include "Bar.h"
#include <memory>

class CUnit : public CPhysicsUnit,
    public CDamageable,
    public CDamager,
    public CTeamMember, 
    public CHealable
{
public:
    CUnit();
    virtual ~CUnit() = default;

    virtual void TakeDamage(float damage, EDamageType damageType) override;
    
    virtual std::shared_ptr<CDamageable> GetDamagable() override;
    virtual std::shared_ptr<CDamager> GetDamager() override;
    virtual std::shared_ptr<CTeamMember> GetTeamMember() override;
    virtual std::shared_ptr<CHealable> GetHealable() override;

    virtual void OnCollision(std::shared_ptr<CPhysicsUnit> physicsUnit) override;
    virtual void EndCollision(std::shared_ptr<CPhysicsUnit> physicsUnit) override;
	virtual void Update(float dt) override;
	virtual void CustomDestroy() override;
	virtual void CustomReset() override;
    virtual void Paint(Color color) override;
    virtual void Heal(HealthType health) override;

private:
    struct SSmoke
    {
        void Show(Vector2 pos);
        void Hide();
        ParticleActor m_actor;
        bool Added = false;
    };

protected:
    void UpdateSmokePosition();

private:
    std::shared_ptr<CBar> m_healthBar;
    SSmoke m_smoke;
    float m_smokeCoef;

private:
    void UpdateHealthBar();
};

class CControlableUnit :
    public CUnit
{
public:
    virtual ~CControlableUnit() = default;

    void HandleAccelerationSignal(float signal);
    void HandleRotationSignal(float signal);

    float GetHorizontalFriction() const;
    void SetHorizontalFriction(float friction);
    float GetAcceleration() const;
    void SetAcceleration(float acceleration);
    float GetMaxForwardSpeed() const;
    void SetMaxForwardSpeed(float maxFwdSpeed);
    float GetMaxBackwardSpeed() const;
    void SetMaxBackwardSpeed(float maxBwdSpeed);
    float GetRotationSpeed() const;
    void SetRotationSpeed(float rotationSpeed);
    float GetAccelerationMultiplyer() const;
    void SetAccelerationMultiplyer(float multiplyer);
    float GetRotationMultiplyer() const;
    void SetRotationMultiplyer(float multiplyer);
    
    virtual void Update(float dt) override;
    virtual void CustomReset() override;

private:
    float m_horizontalFriction;
    float m_acceleration;
    float m_maxForwardSpeed;
    float m_maxBackwardSpeed;
    float m_rotationSpeed;
    float m_accelerationMultiplyer;
    float m_rotationMultiplyer;
};
