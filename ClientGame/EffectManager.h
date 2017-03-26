#pragma once
#include <vector>
#include <memory>
#include "EventManager.h"

#define theEffectManager CEffectManager::GetInstance()

class CEffectManager
{
public:
    static CEffectManager& GetInstance();
    void Update();
    void PlayEffect(EEvent effectType, std::weak_ptr<Actor> source = std::weak_ptr<Actor>{});
    void Stop();
private:
    struct CEffect
    {
        explicit CEffect(EEvent effectType, std::weak_ptr<Actor> source);
        ~CEffect();
        bool Update();
        void UpdatePosition();
        EEvent m_effectType;
        std::weak_ptr<Actor> m_source;
        Actor* m_animation;
    };

private:
    std::vector<std::unique_ptr<CEffect>> m_effects;
    static CEffectManager* ms_instance;
    CEffectManager() = default;
};
