#pragma once
#include "Spawner.h"

class CSpawnerUI:
    public ICooldownSpawnerListener
{
public:
    CSpawnerUI(Vector2 position, Color color, Vector2 size);

    virtual void OnCooldownChanged(float currentCooldown, float cooldown) override;

    virtual ~CSpawnerUI();
 
private:
    std::shared_ptr<Actor> m_circle;
    Vector2 m_size;
};
