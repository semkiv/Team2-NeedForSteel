#pragma once

#include "Common.h"
#include <memory>

class CTeam;
class CPhysicsUnit;

enum class EDamageType
{
    COLLISION,
    BULLET,
    EXPLOSION,
    MELEE
};

using CPath = std::vector<Vector2>;

class IPathFinder
{
public:
    virtual CPath FindPath(Vector2 from, Vector2 to) = 0;
};

class IDamageableListener
{
public:
    virtual void OnTakeDamage(){}
};

class CDamageable
{
public:
    virtual ~CDamageable() = default;

    virtual void TakeDamage(float damage, EDamageType damageType) = 0;

    void OnTakeDamage();
    void AddDamageableListener(std::shared_ptr<IDamageableListener> listener);

    size_t GetWorth() const;
    void SetWorth(size_t worth);

private:
    std::vector<std::weak_ptr<IDamageableListener>> m_damageableListeners;
    size_t m_worth = 0;
};

class IDamagerListener
{
public:
    virtual void OnKilled(std::weak_ptr<CDamageable> victim) = 0;
};

class CDamager
{
public:
    virtual ~CDamager() = default;

    float       GetDamage() const;
    void        SetDamage(float dmg);
    EDamageType GetDamageType() const;
    void        SetDamageType(EDamageType dmgType);
    std::weak_ptr<IDamagerListener> GetDamagerListener() const;
    void SetDamagerListener(std::shared_ptr<IDamagerListener> damagerListener);

    virtual void OnKilled(std::weak_ptr<CDamageable> victim);

private:
    float m_damage;
    EDamageType m_damageType;
    std::weak_ptr<IDamagerListener> m_damagerListener;
};

class CTeamMember
{
public:
    virtual ~CTeamMember() = default;

    std::shared_ptr<CTeam> GetTeam() const;
    void SetTeam(std::shared_ptr<CTeam> team);

    bool IsEnemy(std::shared_ptr<CTeamMember> teamMember);
    virtual void Paint(Color color) {};

private:
    std::shared_ptr<CTeam> m_team;
};

class CTargetEnable
{
public:
    using TargetType = CPhysicsUnit;

    void SetTarget(std::shared_ptr<TargetType> target);
    std::weak_ptr<TargetType> GetTarget() const;
private:
    std::weak_ptr<TargetType> m_target;
};

class CChild
{
public:
    virtual ~CChild() = default;

    using ValueType = CPhysicsUnit;

    std::weak_ptr<ValueType> GetParent();
    void SetParent(std::shared_ptr<ValueType> parent);
private:
    std::weak_ptr<ValueType> m_parent;
};

class CHealable
{
public:
    using HealthType = float;

    void SetMaxHealth(HealthType maxHelath);
    HealthType GetMaxHealth() const;
    HealthType GetHealth() const;
    void SetHealMultiplyer(float healMultiplyer);

    virtual void Heal(HealthType health);
protected:
    float m_healMultiplyer = 1.0f;
    HealthType m_health;
    HealthType m_maxHealth;
};
