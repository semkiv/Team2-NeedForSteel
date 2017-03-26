#include "stdafx.h"
#include "Interfaces.h"
#include "Team.h"
#include "Tools.h"

float CDamager::GetDamage() const
{
    return m_damage;
}

void CDamager::SetDamage(float dmg)
{
    m_damage = dmg;
}

EDamageType CDamager::GetDamageType() const
{
    return m_damageType;
}

void CDamager::SetDamageType(EDamageType dmgType)
{
    m_damageType = dmgType;
}

std::weak_ptr<IDamagerListener> CDamager::GetDamagerListener() const
{
    return m_damagerListener;
}

void CDamager::SetDamagerListener(std::shared_ptr<IDamagerListener> damagerListener)
{
    m_damagerListener = damagerListener;
}

void CDamager::OnKilled(std::weak_ptr<CDamageable> victim)
{
    auto listener = m_damagerListener.lock();
    if (listener != nullptr)
    {
        listener->OnKilled(victim);
    }
}

std::shared_ptr<CTeam> CTeamMember::GetTeam() const
{
    return m_team;
}

void CTeamMember::SetTeam(std::shared_ptr<CTeam> team)
{
    m_team = team;
}

bool CTeamMember::IsEnemy(std::shared_ptr<CTeamMember> teamMember)
{
    return m_team->IsEnemy(teamMember->GetTeam());
}

std::weak_ptr<CChild::ValueType> CChild::GetParent()
{
    return m_parent;
}

void CChild::SetParent(std::shared_ptr<CChild::ValueType> parent)
{
    m_parent = parent;
}

void CHealable::SetMaxHealth(HealthType maxHelath)
{
    m_maxHealth = maxHelath;
}

CHealable::HealthType CHealable::GetHealth() const
{
    return m_health;
}

void CHealable::SetHealMultiplyer(float healMultiplyer)
{
    m_healMultiplyer = healMultiplyer;
}

CHealable::HealthType CHealable::GetMaxHealth() const
{
    return m_maxHealth;
}

void CHealable::Heal(HealthType health)
{
    m_health = MathUtil::Min(m_health + m_healMultiplyer * health, m_maxHealth);
}
 
void CDamageable::OnTakeDamage()
{
    ForeachValidWeak(m_damageableListeners, [](std::shared_ptr<IDamageableListener> listener) 
    {
        listener->OnTakeDamage();
    });
}

void CDamageable::AddDamageableListener(std::shared_ptr<IDamageableListener> listener)
{
    m_damageableListeners.emplace_back(listener);
}

size_t CDamageable::GetWorth() const
{
    return m_worth;
}

void CDamageable::SetWorth(size_t worth)
{
    m_worth = worth;
}
void CTargetEnable::SetTarget(std::shared_ptr<CPhysicsUnit> target)
{
    m_target = target;
}

std::weak_ptr<CPhysicsUnit> CTargetEnable::GetTarget() const
{
    return m_target;
}
