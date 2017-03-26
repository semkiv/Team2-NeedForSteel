#pragma once

#include "../Angel/Infrastructure/Renderable.h"

#include <vector>
#include <memory>

class CLifeTimeManager
{
public:
    virtual ~CLifeTimeManager();

    void ControlLifeTime(std::shared_ptr<Renderable> r);
    void Update();
private:
    std::vector<std::shared_ptr<Renderable>> m_renderables;
};
