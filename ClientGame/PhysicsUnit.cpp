#include "stdafx.h"
#include "PhysicsUnit.h"

void CPhysicsUnit::CustomInitPhysics()
{
    GetBody()->SetUserData(this);
}

void CPhysicsUnit::OnFree()
{
    if (_physBody != nullptr)
    {
        theWorld.GetPhysicsWorld().DestroyBody(_physBody);
    }

    ResetBody();
}

void CPhysicsUnit::OnCollision(std::shared_ptr<CPhysicsUnit> physicsUnit)
{
}

void CPhysicsUnit::EndCollision(std::shared_ptr<CPhysicsUnit> physicsUnit)
{
}

void CPhysicsUnit::SetTransform(Vector2 const& pos, float rotation)
{
    if (GetBody() != nullptr)
    {
        GetBody()->SetTransform(
            { pos.X, pos.Y }, 
            MathUtil::ToRadians(rotation));
    }
}

std::shared_ptr<CDamageable> CPhysicsUnit::GetDamagable()
{
    return nullptr;
}

std::shared_ptr<CDamager> CPhysicsUnit::GetDamager()
{
    return nullptr;
}

std::shared_ptr<CTeamMember> CPhysicsUnit::GetTeamMember()
{
    return nullptr;
}

std::shared_ptr<CHealable> CPhysicsUnit::GetHealable()
{
    return nullptr;
}
