#pragma once
#include "Angel.h"
#include "AIDriver.h"
#include "Interfaces.h"
#include "PhysicsManager.h"

#include <vector>
#include <utility>

class CPhysicsUnit;

class IAIUpdatePath
{
public:
    virtual void UpdatePathToTarget() = 0;
    virtual void DrawPath() {}
};

class IAIUpdateTarget
{
public:
    virtual void UpdateTarget() = 0;
};

class CAITargetDriver :
    public CAIDriver,   
    public IAIUpdatePath,
    public CTargetEnable
{
public:
    void SetDefaultPath(std::vector<Vector2> defaultPath);
    void SetPathFinder(std::shared_ptr<IPathFinder> pathFinder);

    void OnTargetReached();
    virtual void OnDestinationReached() override;
    virtual void UpdatePathToTarget();
    virtual void DrawPath() override;

private:
    std::vector<Vector2> m_defaultPath;
    std::vector<Vector2> m_path;
    std::shared_ptr<IPathFinder> m_pathFinder;
    size_t m_currentPoint;
    size_t m_currentDefaultPoint;

    bool m_useDefaultPath = false;

private:
    void MoveToNextPoint();
    Vector2 CurrentDefaultPoint() const;
};

class CAIUpdateTarget :
    public IAIUpdateTarget,
    public CAI
{
public:
    CAIUpdateTarget(
        std::shared_ptr<CTargetEnable> targetEnableUnit,
        std::shared_ptr<CUnit> detector,
        std::shared_ptr<IClosestFinder> finder);    

    virtual void UpdateTarget() override;
    virtual void Update(float dt) override {};

    void SetRadius(float radius);

private:
    float m_radius;
    std::weak_ptr<CTargetEnable> m_targetEnableUnit;
    std::weak_ptr<CUnit> m_detector;
    std::weak_ptr<IClosestFinder> m_finder;

};
