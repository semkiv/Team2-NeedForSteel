#pragma once
#include "Angel.h"
#include "Common.h"
#include "Player.h"

class CPlayerUI :
    public IPlayerListener
{
public:
    CPlayerUI(Vector2 position);

    virtual void OnGoldChanged(size_t gold) override;
    void SetGold(size_t gold);

    ~CPlayerUI();

private:
    std::shared_ptr<TextActor> m_gold;
};
