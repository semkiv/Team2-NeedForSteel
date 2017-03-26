#include "stdafx.h"
#include "EffectManager.h"
#include "PoolFactory.h"
#include "LuaManager.h"

#include <string>

CEffectManager* CEffectManager::ms_instance = nullptr;

CEffectManager& CEffectManager::GetInstance()
{
    if (ms_instance == nullptr)
    {
        ms_instance = new CEffectManager{};
    }
    return *ms_instance;
}

void CEffectManager::Update()
{
    for (auto it{ m_effects.begin() }; it != m_effects.end();)
    {
        if (!(*it)->Update())
        {
            if (it == --m_effects.cend())
            {
                m_effects.pop_back();
                break;
            }
            else
            {
                *it = std::move(m_effects.back());
                m_effects.pop_back();
            }
        }
        else
        {
            ++it;
        }
    }
}

void CEffectManager::PlayEffect(EEvent effectType, std::weak_ptr<Actor> source)
{
    m_effects.push_back(std::make_unique<CEffect>(effectType, source));
}

void CEffectManager::Stop()
{
    m_effects.clear();
}

CEffectManager::CEffect::CEffect(EEvent effectType, std::weak_ptr<Actor> source):
    m_effectType{ effectType },
    m_source{ source }
{
    auto L = theLuaState("Config/animations.lua");

    float delay{};
    size_t frameCount{};
    std::string firstFilename{};

    switch (effectType)
    {
    case EEvent::SHOT:
        delay = luabridge::getGlobal(L, "shot").At("delay").cast<float>();
        frameCount = luabridge::getGlobal(L, "shot").At("frameCount").cast<size_t>();
        firstFilename = luabridge::getGlobal(L, "shot").At("startFrom").cast<std::string>();
        break;
    case EEvent::EXPLOSION:
        delay = luabridge::getGlobal(L, "explosion").At("delay").cast<float>();
        frameCount = luabridge::getGlobal(L, "explosion").At("frameCount").cast<size_t>();
        firstFilename = luabridge::getGlobal(L, "explosion").At("startFrom").cast<std::string>();
        break;
    case EEvent::MINE_EXPLOSION:
        delay = luabridge::getGlobal(L, "mineExplosion").At("delay").cast<float>();
        frameCount = luabridge::getGlobal(L, "mineExplosion").At("frameCount").cast<size_t>();
        firstFilename = luabridge::getGlobal(L, "mineExplosion").At("startFrom").cast<std::string>();
        break;
    }

    m_animation = CPoolFactory<Actor>::GetInstance().Allocate();
    theWorld.Add(m_animation, static_cast<int>(ELayer::VISUAL_EFFECT));

    m_animation->LoadSpriteFrames(firstFilename);
    m_animation->PlaySpriteAnimation(delay, SAT_OneShot, 0, frameCount);

    UpdatePosition();
}

CEffectManager::CEffect::~CEffect()
{
    theWorld.Remove(m_animation);
    m_animation->ClearSpriteInfo();
    CPoolFactory<Actor>::GetInstance().Deallocate(m_animation);
}

bool CEffectManager::CEffect::Update()
{
    if (!m_animation || !m_animation->IsSpritePlaying())
    {
        return false;
    }

    return true;
}

void CEffectManager::CEffect::UpdatePosition()
{
    auto sourcePtr = m_source.lock();
    if (sourcePtr)
    {
        switch (m_effectType)
        {
        case EEvent::SHOT:
            m_animation->SetPosition(sourcePtr->GetPosition() + Vector2(cos(MathUtil::ToRadians(sourcePtr->GetRotation())),
                sin(MathUtil::ToRadians(sourcePtr->GetRotation()))) * (sourcePtr->GetSize().X + m_animation->GetSize().X) * 0.5f);
            m_animation->SetRotation(sourcePtr->GetRotation());
            break;
        default:
            m_animation->SetPosition(sourcePtr->GetPosition());
        }
    }
}
