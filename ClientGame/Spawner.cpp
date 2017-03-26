#include "stdafx.h"
#include "Spawner.h"

CPositionalSpawner::CPositionalSpawner():
    m_position(Vector2::Zero),
    m_rotation(0.0f)
{
}

void CPositionalSpawner::SetSpawnPosition(Vector2 position)
{
    m_position = position;
}

Vector2 CPositionalSpawner::GetSpawnPosition() const
{
    return m_position;
}

void CPositionalSpawner::SetSpawnRotation(float rotation)
{
    m_rotation = rotation;
}

float CPositionalSpawner::GetSpawnRotatoin() const
{
    return m_rotation;
}

CCooldownSpawner::CCooldownSpawner():
    m_currentTime{ 0.0f },
    m_cooldown{ 0.0f }
{
}

void CCooldownSpawner::Cooldown()
{
    m_currentTime = m_cooldown;
}

void CCooldownSpawner::Update(float dt)
{
    if (m_currentTime > 0)
    {
        m_currentTime -= dt;
        auto listener = m_listener.lock();
        if (listener != nullptr)
        {
            listener->OnCooldownChanged(m_currentTime, m_cooldown);
        }
        if (m_currentTime <= 0)
        {
            Spawn();
        }
    }
}

bool CCooldownSpawner::OnCooldown() const
{
    return m_currentTime > 0;
}

void CCooldownSpawner::SetCooldown(float cooldown)
{
    m_cooldown = cooldown;
}

void CCooldownSpawner::SetCooldownListener(std::shared_ptr<ICooldownSpawnerListener> listener)
{
    m_listener = listener;
}

void CPhysicsUnitCooldownSpawner::Spawn()
{       
    m_unit->SetTransform(
        GetSpawnPosition(),
        GetSpawnRotatoin()
    );

    m_unit->GetBody()->SetActive(true);
    m_unit->Reset();
}

void CPhysicsUnitCooldownSpawner::Update(float dt)
{
    if (!OnCooldown())
    {
        if (m_unit->IsDestroyed())
        {
            m_unit->GetBody()->SetActive(false);
            m_unit->GetBody()->SetLinearVelocity(b2Vec2{ 0.0f, 0.0f });
            m_unit->GetBody()->SetAngularVelocity(0.0f);
            Cooldown();
        }
    }

    CCooldownSpawner::Update(dt);
}

void CPhysicsUnitCooldownSpawner::SetUnit(std::shared_ptr<CPhysicsUnit> unit)
{
    m_unit = unit;
}

CTimeSpawner::CTimeSpawner():
    m_currentTime{ 0.0f },
    m_spawnTime{ 0.0f }
{
}

void CTimeSpawner::SetSpawnTime(float spawnTime)
{
    m_spawnTime = spawnTime;
}

void CTimeSpawner::Update(float dt)
{
    if (m_currentTime > 0)
    {
        m_currentTime -= dt;
        if (m_currentTime <= 0)
        {
            Spawn();
        }
    }
    else
    {
        m_currentTime = m_spawnTime;
    }
}

void CSpawnManager::AddSpawner(std::shared_ptr<ISpawner> spawner)
{
    m_spawners.push_back(spawner);
}

void CSpawnManager::Update(float dt)
{
    for (auto& spawner : m_spawners)
    {
        spawner->Update(dt);
    }
}
