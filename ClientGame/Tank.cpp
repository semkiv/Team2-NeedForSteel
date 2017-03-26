#include "stdafx.h"

#include "Tank.h"

std::shared_ptr<CTurret> CTank::GetTurret() const
{
    return m_turret;
}

void CTank::SetTurret(std::shared_ptr<CTurret> turret)
{
    AddComponent(turret);

    turret->SetParent(std::static_pointer_cast<CTank>(shared_from_this()));
    m_turret = turret;
}

float CTank::GetFeedBack() const
{
    return m_feedback;
}

void CTank::SetFeedBack(float feedback)
{
    m_feedback = feedback;
}

void CTank::HandleTurretRotationSignal(float signal)
{
    m_turret->HandleRotationSignal(signal);
}

void CTank::HandleShootSignal(float signal)
{
    if (signal == 1.0f && m_turret->IsReloaded())
    {
        m_turret->Shoot();

        auto currentVelocity = GetBody()->GetLinearVelocity();
        float vx = currentVelocity.x - m_feedback * cos(MathUtil::ToRadians(m_turret->GetRelativeRotation())) * cos(MathUtil::ToRadians(GetRotation()));
        float vy = currentVelocity.y - m_feedback * cos(MathUtil::ToRadians(m_turret->GetRelativeRotation())) * sin(MathUtil::ToRadians(GetRotation()));
        GetBody()->SetLinearVelocity(b2Vec2(vx, vy));
    }
}

void CTank::Update(float dt)
{
    CControlableUnit::Update(dt);    
    
    m_turret->SetPosition(GetPosition());
    m_turret->SetRotation(m_turret->GetRotation() + MathUtil::ToDegrees(GetRotationMultiplyer() * GetRotationSpeed() * dt));
}

void CTank::OnFree()
{
    m_turret.reset();
    CControlableUnit::OnFree();
}

void CTank::CustomReset()
{
    CControlableUnit::CustomReset();

    HandleRotationSignal(0.0f);
    HandleShootSignal(0.0f);   

    if (m_turret != nullptr)
    {
        HandleTurretRotationSignal(0.0f);
        m_turret->SetRotationMultiplyer(0.0f);
        m_turret->SetRelativeRotation(0.0f);
    }
}

void CTank::SetChildrenPositions(Vector2 const& pos)
{
    SetPosition(pos);

    if (m_turret != nullptr)
    {
        m_turret->SetChildrenPositions(pos);
    }
}

void CTank::SetChildrenColors(Color const& color)
{
    SetColor(color);

    if (m_turret != nullptr)
    {
        m_turret->SetChildrenColors(color);
    }
}

void CTank::SetChildrenRotations(float rotation)
{
    SetRotation(rotation);

    if (m_turret != nullptr)
    {
        m_turret->SetChildrenRotations(rotation);
    }
}

void CTank::SetTransform(Vector2 const& pos, float rotation)
{
    SetChildrenPositions(pos);
    SetChildrenRotations(rotation);

    CControlableUnit::SetTransform(pos, rotation);
}
