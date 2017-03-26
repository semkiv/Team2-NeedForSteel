#pragma once
#include "Angel.h"
#include "PhysicsUnit.h"
#include <memory>
#include <vector>

class ISpawner
{
public:
    virtual void Spawn() = 0;
    virtual void Update(float dt) = 0;
};

class CPositionalSpawner :
    public ISpawner
{
public:
    CPositionalSpawner();

    void SetSpawnPosition(Vector2 position);
    Vector2 GetSpawnPosition() const;
    void SetSpawnRotation(float rotation);
    float GetSpawnRotatoin() const;

private:
	Vector2 m_position;
	float m_rotation;
};

class ICooldownSpawnerListener
{
public:
    virtual void OnCooldownChanged(float currentCooldown, float cooldown) = 0;
};

class CCooldownSpawner :
    public CPositionalSpawner
{
public:
    CCooldownSpawner();

    void Cooldown();
    virtual void Update(float dt) override;

    bool OnCooldown() const;
    void SetCooldown(float cooldown);

    void SetCooldownListener(std::shared_ptr<ICooldownSpawnerListener> listener);

private:
    float m_cooldown;
    float m_currentTime;

    std::weak_ptr<ICooldownSpawnerListener> m_listener;
};

class CPhysicsUnitCooldownSpawner :
    public CCooldownSpawner
{
public:
    virtual void Spawn();
    virtual void Update(float dt) override;

    void SetUnit(std::shared_ptr<CPhysicsUnit> unit);    

private:
    std::shared_ptr<CPhysicsUnit> m_unit;
};

class CTimeSpawner: 
    public CPositionalSpawner
{
public:
    CTimeSpawner();

    void SetSpawnTime(float spawnTime);
    virtual void Update(float dt) override;

private:
    float m_spawnTime;
    float m_currentTime;
};

template<typename Func>
class CFunctionalSpawner:
    public CTimeSpawner
{
public: 
    CFunctionalSpawner(Func func):
        m_func(func)
    {
        
    }

    virtual void Spawn() override 
    {
        m_func(*this);
    }

private:
    Func m_func;

};

class CSpawnManager
{
public:
    void AddSpawner(std::shared_ptr<ISpawner> spawner);
    void Update(float dt);

private:
    std::vector<std::shared_ptr<ISpawner>> m_spawners;
};
