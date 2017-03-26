#include "stdafx.h"
#include "Mine.h"
#include "Game.h"

void CMine::Update(float dt)
{
    CControlableUnit::Update(dt);
}

void CMine::CustomDestroy()
{
    try
    {
        theEventManager.Trigger(EEvent::MINE_EXPLOSION, shared_from_this());
    }
    catch (std::bad_weak_ptr)
    {
    }

    CControlableUnit::CustomDestroy();
    Boom();


}

void CMine::TakeDamage(float damage, EDamageType damageType)
{
     if (damageType == EDamageType::MELEE)
    {
        Destroy();
    }
    else
    {
        CControlableUnit::TakeDamage(damage, damageType);
    }
}

void CMine::Boom()
{
    if (theGame.GameStopped())
    {
        return;
    }

    auto units = theGame.GetPhysicsManager()->FindAllInsideCircle(GetPosition(), m_damageRadius);
    float distance = 0.0f;
    float c = - m_explosionDamage / (m_damageRadius * m_damageRadius);

    for (auto& unit : units)
    {
        if (unit.get() != this && !unit->IsDestroyed())
        {
            if (unit->GetDamagable() == nullptr)
            {
                continue;
            }    

            distance = (unit->GetPosition() - GetPosition()).LengthSquared();
            unit->GetDamagable()->TakeDamage(m_explosionDamage + c * distance, EDamageType::EXPLOSION);
        }
    }
}

void CMine::SetExplosionDamage(float explosionDamage)
{
    m_explosionDamage = explosionDamage;
}

void CMine::SetDamageRadius(float damageRadius)
{
    m_damageRadius = damageRadius;
}
