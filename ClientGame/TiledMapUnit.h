#pragma once

#include "Angel.h"
#include <memory>

class CTiledMapCell;

class CTiledMapUnit
{
public:
    CTiledMapUnit(std::shared_ptr<Actor> actor);

private:
    std::weak_ptr<Actor> m_actor;

};
