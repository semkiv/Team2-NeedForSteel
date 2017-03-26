#pragma once

#include "Player.h"

#include <vector>
#include <memory>

class CControllerEffectManager
{
public:
    void AddControllerEffectsToPlayer(size_t playerId, std::shared_ptr<CPlayer> player);

private:
    std::vector<std::shared_ptr<IPlayerListener>> m_playersListener;
};
