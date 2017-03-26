#include "stdafx.h"
#include "Turret.h"
#include "Factory.h"
#include "Projectile.h"
#include "Team.h"
#include "Game.h"
#include "EffectManager.h"

CTurret::CTurret()
{
    m_cdBar.SetColor({ 0.f, 1.f, 1.f, 1.f });
    m_cdBar.SetBackgroundColor({ 0.f, 0.f, 0.f, 0.f });  
}

void CTurret::Shoot()
{
    auto base = GetParent().lock(); 
    
    if (IsReloaded() && base != nullptr)
    {
        theEventManager.Trigger(EEvent::SHOT, shared_from_this());

        auto bullet = theGame.CreateUnit<CProjectile>(GetProjectile());
        bullet->SetTeam(base->GetTeamMember()->GetTeam());
        bullet->SetPosition(GetPosition() + Vector2(cos(MathUtil::ToRadians(GetRotation())), sin(MathUtil::ToRadians(GetRotation()))) * GetSize().X / 2);
        bullet->SetRotation(GetRotation());
        bullet->SetDamage(GetDamage());
        bullet->SetDamageType(GetDamageType());
        bullet->SetFlightDistance(GetProjectileFlightDistance());				
        bullet->SetStartingPoint(bullet->GetPosition());
        bullet->SetOwner(base);
        bullet->InitPhysics();
        bullet->GetBody()->SetLinearVelocity(b2Vec2(GetProjectileSpeed() * cos(MathUtil::ToRadians(GetRotation())), GetProjectileSpeed() * sin(MathUtil::ToRadians(GetRotation()))));
        
        Reload();
    }
}

void CTurret::HandleRotationSignal(float signal)
{
    m_rotationMultiplyer = signal;
}

void CTurret::CustomReset()
{
    CRangeWeapon::CustomReset();
    m_cdBar.Show();
}

void CTurret::CustomDestroy()
{
    CRangeWeapon::CustomDestroy();
    m_cdBar.Hide();
}

float CTurret::GetRelativeRotation() const
{
    return m_relativeRotation;
}

void CTurret::SetRelativeRotation(float angle)
{
    m_relativeRotation = angle;
}

float CTurret::GetRotationSpeed() const
{
    return m_rotationSpeed;
}

void CTurret::SetRotationSpeed(float rotationSpeed)
{
    m_rotationSpeed = rotationSpeed;
}

float CTurret::GetRotationMultiplyer() const
{
    return m_rotationMultiplyer;
}

void CTurret::SetRotationMultiplyer(float multiplyer)
{
    m_rotationMultiplyer = multiplyer;
}

float CTurret::GetProjectileSpeed() const
{
    return m_projectileSpeed;
}

void CTurret::SetProjectileSpeed(float speed)
{
    m_projectileSpeed = speed;
}

float CTurret::GetProjectileFlightDistance() const
{
    return m_projectileFlightDistance;
}

void CTurret::SetProjectileFlightDistance(float distance)
{
    m_projectileFlightDistance = distance;
}

void CTurret::SetProjectile(std::string const& projectile)
{
    m_projectile = projectile;
}

std::string const& CTurret::GetProjectile() const
{
    return m_projectile;
}

void CTurret::Update(float dt)
{
    CRangeWeapon::Update(dt);
    m_relativeRotation += MathUtil::ToDegrees(GetRotationSpeed() * GetRotationMultiplyer() * dt);
    auto parent = GetParent().lock();
    assert(parent != nullptr);
    SetRotation(parent->GetRotation() + m_relativeRotation);  

    UpdateCooldownBar();
}

void CTurret::UpdateCooldownBar()
{
    auto parent = GetParent().lock();
    assert(parent != nullptr);
    float y = parent->GetPosition().Y + m_cdBar.GetSize().Y * 0.5f + parent->GetSize().Y * 0.5f + 0.1f;
    m_cdBar.SetPosition(Vector2{ parent->GetPosition().X, y });
    m_cdBar.SetValue(m_currentCoolDown / m_coolDown);
}

void CTargetTurret::Update(float dt)
{
    CRangeWeapon::Update(dt);
    UpdateCooldownBar();

    auto target = GetTarget().lock();
    if (target == nullptr)
    {
        return;
    }

    if (MathUtil::DeltaAngle(MathUtil::AngleFromVector(target->GetPosition() - GetPosition()), MathUtil::ToRadians(GetRotation())) > 0.01 * GetRotationSpeed())
    {
        SetRotation(GetRotation() - MathUtil::ToDegrees(GetRotationSpeed() * dt));
    }
    else if (MathUtil::DeltaAngle(MathUtil::AngleFromVector(target->GetPosition() - GetPosition()), MathUtil::ToRadians(GetRotation())) < -0.01 * GetRotationSpeed())
    {
        SetRotation(GetRotation() + MathUtil::ToDegrees(GetRotationSpeed() * dt));
    }
    else
    {
        if (IsReloaded())
        {
            Shoot();
        }
    }
}
