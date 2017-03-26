#include "stdafx.h"
#include "Projectile.h"
#include "Factory.h"
#include "Team.h"

CBasicProjectile::CBasicProjectile()
{
    TagBit(static_cast<size_t>(ETag::PROJECTILE));
    UntagBit(static_cast<size_t>(ETag::TARGETABLE));

    SetDamageType(EDamageType::BULLET);
}

void CBasicProjectile::CustomInitPhysics()
{
    CPhysicsUnit::CustomInitPhysics();
    GetBody()->SetBullet(true);
}

void CBasicProjectile::PreCustomInitPhysics()
{
    SetIsSensor(true);
}

void CBasicProjectile::CustomDestroy()
{
    try
    {
        theEventManager.Trigger(EEvent::EXPLOSION, shared_from_this());
    }
    catch(std::bad_weak_ptr)
    {
    }
}

Vector2 CBasicProjectile::GetStartingPoint() const
{
    return m_startingPoint;
}

void CBasicProjectile::SetStartingPoint(Vector2 const& point)
{
    m_startingPoint = point;
}

std::weak_ptr<CPhysicsUnit> CBasicProjectile::GetOwner() const
{
    return m_owner;
}

void CBasicProjectile::SetOwner(std::shared_ptr<CPhysicsUnit> owner)
{
    m_owner = owner;
}

void CBasicProjectile::TakeDamage(float damage, EDamageType damageType)
{
    Destroy();
}

std::shared_ptr<CDamageable> CBasicProjectile::GetDamagable()
{
    if (IsDestroyed())
    {
        return nullptr;
    }

    return std::static_pointer_cast<CBasicProjectile>(shared_from_this());
}

std::shared_ptr<CDamager> CBasicProjectile::GetDamager()
{
    if (IsDestroyed())
    {
        return nullptr;
    }

    return std::static_pointer_cast<CBasicProjectile>(shared_from_this());
}

std::shared_ptr<CTeamMember> CBasicProjectile::GetTeamMember()
{
    if (IsDestroyed())
    {
        return nullptr;
    }

    return std::static_pointer_cast<CBasicProjectile>(shared_from_this());
}

void CBasicProjectile::OnCollision(std::shared_ptr<CPhysicsUnit> physicsUnit)
{
    if (IsDestroyed())
    {
        return;
    }

    if (!physicsUnit->IsBitTagged(static_cast<size_t>(ETag::PROJECTILE)))
    {
        physicsUnit->OnCollision(shared_from_this());
    }
    else
    {
        Destroy();
        physicsUnit->Destroy();
    }
}

void CBasicProjectile::OnKilled(std::weak_ptr<CDamageable> victim)
{
    auto v = victim.lock();
    auto base = GetOwner().lock();
    if (v != nullptr && base != nullptr && base->GetDamager() != nullptr)
    {
        base->GetDamager()->OnKilled(victim);
    }
}

float CProjectile::GetFlightDistance() const
{
    return m_flightDistance;
}

void CProjectile::SetFlightDistance(float distance)
{
    m_flightDistance = distance;
}

void CProjectile::Update(float dt)
{
    if ((GetPosition() - GetStartingPoint()).Length() > m_flightDistance)
    {
        Destroy();
    }
}

std::weak_ptr<CPhysicsUnit> CTargetProjectile::GetTarget() const
{
    return m_target;
}

void CTargetProjectile::SetTarget(std::shared_ptr<CPhysicsUnit> target)
{
    m_target = target;
}

void CTargetProjectile::Update(float dt)
{
    auto target = m_target.lock();

    if ((target != nullptr && !target->IsDestroyed()) && (target->GetPosition() - GetPosition()).Length() > 0.0f)
    {
        float targetDirection = MathUtil::AngleFromVector(target->GetPosition() - GetPosition());

        float speed = GetBody()->GetLinearVelocity().Length();
        GetBody()->SetLinearVelocity(b2Vec2(speed * cos(targetDirection), speed * sin(targetDirection)));

        float relativeAngle = MathUtil::DeltaAngle(targetDirection, MathUtil::ToRadians(GetRotation()));

        if (relativeAngle > 0.01f)
        {
            GetBody()->SetAngularVelocity(-0.5f);
        }
        else if (relativeAngle < -0.01f)
        {
            GetBody()->SetAngularVelocity(+0.5f);
        }
        else
        {
            GetBody()->SetAngularVelocity(0.0f);
        }
    }
    else
    {
        Destroy();
    }
}
