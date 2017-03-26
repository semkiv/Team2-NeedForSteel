#pragma once

enum  class EEvent
{
    SHOT,
    EXPLOSION,
    MINE_EXPLOSION
};

#define theEventManager CEventManager::GetInstance()

class CEventManager
{
public:
    static CEventManager& GetInstance();
    void Trigger(EEvent event, std::weak_ptr<Actor> source = std::weak_ptr<Actor>{}) const;
    void Update();
private:
    CEventManager() = default;
    static CEventManager* ms_instance;
};
