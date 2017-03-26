#pragma once
#include "PhysicsUnit.h"

class CObstacle : public CPhysicsUnit
{

};

class CWall: public CObstacle
{
public:
    virtual void OnCollision(std::shared_ptr<CPhysicsUnit> physicsUnit) override;
};

class CBarricade : public CObstacle
{
};
