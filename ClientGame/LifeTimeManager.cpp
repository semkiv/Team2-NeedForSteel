#include "stdafx.h"
#include "LifeTimeManager.h"

CLifeTimeManager::~CLifeTimeManager()
{
    for (auto& renderable: m_renderables)
    {
        renderable->SetListener(nullptr);
    }
}

void CLifeTimeManager::ControlLifeTime(std::shared_ptr<Renderable> r)
{
    m_renderables.push_back(r);
}

void CLifeTimeManager::Update()
{
    for(size_t i = 0; i < m_renderables.size();)
    {
        if (m_renderables[i]->IsDestroyed())
        {
            m_renderables[i] = m_renderables.back();
            m_renderables.pop_back();
        }
        else
        {
            ++i;
        }
    }
}
