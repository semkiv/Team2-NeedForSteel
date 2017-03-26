#pragma once

#include "PhysicsUnit.h"
#include "Tools.h"
#include "Team.h"
#include <memory>
#include <vector>

class IClosestFinder
{
public:
    virtual std::shared_ptr<CPhysicsUnit> FindClosestInsideCircle(Vector2 position, float radius, CTeam::MaskType mask) = 0;
};

class CPhysicsManager :
    public IClosestFinder
{
public:    
    void AddPhysicsUnit(std::shared_ptr<CPhysicsUnit> unit);
    
    std::vector<std::shared_ptr<CPhysicsUnit>> FindAllInsideCircle(Vector2 position, float radius);
    virtual std::shared_ptr<CPhysicsUnit> FindClosestInsideCircle(Vector2 position, float radius, CTeam::MaskType mask) override;

private:
    std::vector<std::weak_ptr<CPhysicsUnit>> m_units;
};
