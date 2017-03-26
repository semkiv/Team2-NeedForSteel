#include "stdafx.h"
#include "AIManager.h"
#include "Tools.h"

CAIManager::CAIManager():
    m_handleAIPerTime{ 4 },
    m_currentUpdatePathTime{ 0.0f },
    m_currentUpdateTargetTime{ 0.0f }
{
}

void CAIManager::Update(float dt)
{
    UpdateAI(dt);
    UpdateTargetAI(dt);
    UpdatePathAI(dt);    
}

void CAIManager::SetUpdatePathTime(float updatePathTime)
{
    m_updatePathTime = updatePathTime;
}

void CAIManager::SetUpdateTargetTime(float updateTargetTime)
{
    m_updateTargetTime = updateTargetTime;
}

void CAIManager::AddAI(std::shared_ptr<CAI> ai)
{
    m_aiUnits.push_back(ai);
}

void CAIManager::AddUpdatePathAI(std::shared_ptr<IAIUpdatePath> ai)
{
    m_updatePathAis.push_back(ai);
}

void CAIManager::AddUpdateTargetAI(std::shared_ptr<IAIUpdateTarget> ai)
{
    m_updateTargetAis.emplace_back(ai);
}

void CAIManager::DrawPath()
{
    glColor3f(1.0f, 0.0f, 0.0f);
    ForeachValidWeak(m_updatePathAis, [](std::shared_ptr<IAIUpdatePath> ai)
    {
        ai->DrawPath();
    });
}

void CAIManager::AddPathAIToQueue()
{
    ForeachValidWeak(m_updatePathAis, [this](std::shared_ptr<IAIUpdatePath> ai)
    {
        m_updatePathAisQueue.push(ai);
    });
}

void CAIManager::AddTargetAIToQueue()
{
    ForeachValidWeak(m_updateTargetAis, [this](std::shared_ptr<IAIUpdateTarget> ai)
    {
        m_updateTargetAisQueue.push(ai);
    });
}

void CAIManager::UpdateAI(float dt)
{
    std::shared_ptr<CAI> ai = nullptr;

    for (size_t i = 0; i < m_aiUnits.size(); ++i)
    {
        ai = m_aiUnits[i];
        ai->Update(dt);

        if (ai->IsEmpty())
        {
            m_aiUnits[i] = std::move(m_aiUnits.back());
            m_aiUnits.pop_back();
        }       
    }
}

void CAIManager::UpdatePathAI(float dt)
{
    if (m_currentUpdatePathTime > 0.0f)
    {
        m_currentUpdatePathTime -= dt;
        if (m_currentUpdatePathTime <= 0.0f)
        {
            AddPathAIToQueue();
        }
    }
    else
    {
        m_currentUpdatePathTime = m_updatePathTime - dt;
    }

    for (size_t i = 0; i < m_handleAIPerTime && !m_updatePathAisQueue.empty(); ++i)
    {
        auto ai = m_updatePathAisQueue.front().lock();

        if (ai != nullptr)
        {
            ai->UpdatePathToTarget();
        }

        m_updatePathAisQueue.pop();
    }
}

void CAIManager::UpdateTargetAI(float dt)
{
    if (m_currentUpdateTargetTime > 0.0f)
    {
        m_currentUpdateTargetTime -= dt;
        if (m_currentUpdateTargetTime <= 0.0f)
        {
            AddTargetAIToQueue();
        }
    }
    else
    {
        m_currentUpdateTargetTime = m_updateTargetTime - dt;
    }

    for (size_t i = 0; i < m_handleAIPerTime && !m_updateTargetAisQueue.empty(); ++i)
    {
        auto ai = m_updateTargetAisQueue.front().lock();

        if (ai != nullptr)
        {
            ai->UpdateTarget();
        }

        m_updateTargetAisQueue.pop();
    }
}
