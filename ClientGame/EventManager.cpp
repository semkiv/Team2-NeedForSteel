#include "stdafx.h"
#include "EventManager.h"
#include "EffectManager.h"
#include "SoundManager.h"

CEventManager* CEventManager::ms_instance = nullptr;

CEventManager& CEventManager::GetInstance()
{
    if (ms_instance == nullptr)
    {
        ms_instance = new CEventManager{};
    }
    return *ms_instance;
}

void CEventManager::Trigger(EEvent event, std::weak_ptr<Actor> source) const
{
    switch (event)
    {
    case EEvent::SHOT:
        theEffectManager.PlayEffect(event, source);
        theSoundManager.PlaySound(event, source);
        break;
    case EEvent::EXPLOSION:
        theEffectManager.PlayEffect(event, source);
        break;
    case EEvent::MINE_EXPLOSION:
        theEffectManager.PlayEffect(event, source);
        break;
    }
}

void CEventManager::Update()
{
    theSoundManager.Update();
    theEffectManager.Update();
}
