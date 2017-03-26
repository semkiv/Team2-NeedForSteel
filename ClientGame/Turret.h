#pragma once

#include "Angel.h"
#include "Unit.h"
#include "Weapon.h"
#include "TargetDetection.h"
#include "Bar.h"

#include <memory>

class CTurret : public CRangeWeapon,
    public CDamager,    
    public CChild,
    public std::enable_shared_from_this<CTurret>
{
public:
    CTurret();
    virtual ~CTurret() = default;

    virtual void Shoot();
    void HandleRotationSignal(float signal);

    virtual void CustomReset() override;
    virtual void CustomDestroy() override;
    float GetRelativeRotation() const;
    void SetRelativeRotation(float angle);
    float GetRotationSpeed() const;
    void SetRotationSpeed(float rotationSpeed);
    float GetRotationMultiplyer() const;
    void SetRotationMultiplyer(float multiplyer);
    float GetProjectileSpeed() const;
    void SetProjectileSpeed(float speed);
    float GetProjectileFlightDistance() const;
    void SetProjectileFlightDistance(float distance);

    void SetProjectile(std::string const& projectile);
    std::string const& GetProjectile() const;

    virtual void Update(float dt) override;

protected:
    void UpdateCooldownBar();

private:
    float m_relativeRotation;
    float m_rotationSpeed;
    float m_rotationMultiplyer;

    float m_projectileSpeed;
    float m_projectileFlightDistance;
    std::string m_projectile;
    CCoolDownBar m_cdBar;
};

class CTargetTurret: 
    public CTurret,
    public CTargetEnable
{
public:
    virtual ~CTargetTurret() = default;

    virtual void Update(float dt) override;
};
