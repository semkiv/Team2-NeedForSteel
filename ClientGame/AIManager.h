#pragma once

#include "AITargetDriver.h"
#include <vector>
#include <memory>
#include <queue>

class CAIManager
{
public:
    CAIManager();

    void Update(float dt);
    void SetUpdatePathTime(float updatePathTime);
    void SetUpdateTargetTime(float updateTargetTime);

    void AddAI(std::shared_ptr<CAI> ai);
    void AddUpdatePathAI(std::shared_ptr<IAIUpdatePath> ai);
    void AddUpdateTargetAI(std::shared_ptr<IAIUpdateTarget> ai);

    void DrawPath();

private:
    std::vector<std::shared_ptr<CAI>> m_aiUnits;
    std::vector<std::weak_ptr<IAIUpdatePath>> m_updatePathAis;
    std::vector<std::weak_ptr<IAIUpdateTarget>> m_updateTargetAis;
    
    std::queue<std::weak_ptr<IAIUpdatePath>> m_updatePathAisQueue;
    std::queue<std::weak_ptr<IAIUpdateTarget>> m_updateTargetAisQueue;

    size_t m_handleAIPerTime;
    float m_currentUpdatePathTime;
    float m_updatePathTime;
    float m_currentUpdateTargetTime;
    float m_updateTargetTime;

private:
    void AddPathAIToQueue();
    void AddTargetAIToQueue();

    void UpdateAI(float dt);
    void UpdatePathAI(float dt);
    void UpdateTargetAI(float dt);

};
