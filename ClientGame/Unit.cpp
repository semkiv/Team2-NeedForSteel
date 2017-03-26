#include "stdafx.h"

#include "Unit.h"
#include "Factory.h"
#include "Team.h"
#include "LuaManager.h"

CUnit::CUnit():
    m_smokeCoef(0.51f)
{
    TagBit(static_cast<size_t>(ETag::TARGETABLE));
    TagBit(static_cast<size_t>(ETag::DETECTABLE));
    m_healthBar = std::make_shared<CBar>();
    m_healthBar->SetBackgroundColor({ 0.0f, 0.0f, 0.0f, 0.0f });

    UpdateHealthBar();

    luabridge::lua_State* L = luabridge::luaL_newstate();
    luabridge::luaL_openlibs(L);
    luaL_dofile(L, "Config/smoke.lua");

    auto color = luabridge::getGlobal(L, "color");
    auto endColor = luabridge::getGlobal(L, "endColor");
    auto sprite = luabridge::getGlobal(L, "sprite").cast<std::string>();
    auto length = luabridge::getGlobal(L, "size")["length"].cast<float>();
    auto width = luabridge::getGlobal(L, "size")["width"].cast<float>();
    auto maxParticles = luabridge::getGlobal(L, "maxParticles").cast<size_t>();
    auto emissionRate = luabridge::getGlobal(L, "emissionRate").cast<float>();
    auto lifeTime = luabridge::getGlobal(L, "lifeTime").cast<float>();
    auto spreadAngle = luabridge::getGlobal(L, "spreadAngle").cast<float>();
    auto endScale = luabridge::getGlobal(L, "endScale").cast<float>();
    auto attractorStrength = luabridge::getGlobal(L, "attractorStrength").cast<float>();
    auto minSpeed = luabridge::getGlobal(L, "minSpeed").cast<float>();
    auto maxSpeed = luabridge::getGlobal(L, "maxSpeed").cast<float>();

    m_smoke.m_actor.SetColor(color.At("red").cast<float>(), color.At("green").cast<float>(), color.At("blue").cast<float>(), color.At("alpha").cast<float>());
    m_smoke.m_actor.SetEndColor(Color(endColor.At("red").cast<float>(), endColor.At("green").cast<float>(), endColor.At("blue").cast<float>(), endColor.At("alpha").cast<float>()));
    m_smoke.m_actor.SetSize(length, width);
    m_smoke.m_actor.SetSprite(sprite);
    m_smoke.m_actor.SetMaxParticles(maxParticles);
    m_smoke.m_actor.SetParticlesPerSecond(emissionRate);
    m_smoke.m_actor.SetParticleLifetime(lifeTime);
    m_smoke.m_actor.SetSpread(spreadAngle);
    m_smoke.m_actor.SetEndScale(endScale);
    m_smoke.m_actor.SetAttractorStrength(attractorStrength);
    m_smoke.m_actor.SetSpeedRange(minSpeed, maxSpeed);
    m_smoke.m_actor.SetGravity(Vector2::Zero);
    m_smoke.m_actor.SetPosition(GetPosition());

}

void CUnit::TakeDamage(float damage, EDamageType damageType)
{
    if(m_health <= damage)
    {
        m_health = 0u;

        if (!IsDestroyed())
        {
            Destroy();
        }
    }
    else
    {
        m_health -= damage;
        if (m_health / GetMaxHealth() < m_smokeCoef)
        {
            m_smoke.Show(GetPosition());
        }
    }

    if (damage > 0)
    {
        OnTakeDamage();
    }
}

std::shared_ptr<CDamageable> CUnit::GetDamagable()
{
    if (IsDestroyed())
    {
        return nullptr;
    }

    return std::static_pointer_cast<CUnit>(shared_from_this());
}

std::shared_ptr<CDamager> CUnit::GetDamager()
{
    if (IsDestroyed())
    {
        return nullptr;
    }

    return std::static_pointer_cast<CUnit>(shared_from_this());
}

std::shared_ptr<CTeamMember> CUnit::GetTeamMember()
{
    if (IsDestroyed())
    {
        return nullptr;
    }

    return std::static_pointer_cast<CUnit>(shared_from_this());
}

std::shared_ptr<CHealable> CUnit::GetHealable()
{
    if (IsDestroyed())
    {
        return nullptr;
    }

    return std::static_pointer_cast<CUnit>(shared_from_this());
}

void CUnit::OnCollision(std::shared_ptr<CPhysicsUnit> physicsUnit)
{
    if (IsDestroyed())
    {
        return;
    }

    if (physicsUnit->IsBitTagged(static_cast<size_t>(ETag::DETECTOR)))
    {
        physicsUnit->OnCollision(shared_from_this());
        return;
    }

    auto teamMember = physicsUnit->GetTeamMember();
    if (teamMember == nullptr)
    {
        return;
    }

    if (teamMember->IsEnemy(GetTeamMember()))
    {
        auto damager = physicsUnit->GetDamager();
        if (damager != nullptr)
        {
            TakeDamage(damager->GetDamage(), damager->GetDamageType());
        }

        if (m_health == 0u)
        {
            damager->OnKilled(std::static_pointer_cast<CUnit>(shared_from_this()));
        }

        auto damageable = physicsUnit->GetDamagable();
        if (damageable != nullptr)
        {
            damageable->TakeDamage(GetDamage(), GetDamageType());
        }
    }
}

void CUnit::EndCollision(std::shared_ptr<CPhysicsUnit> physicsUnit)
{
    if (physicsUnit->IsBitTagged(physicsUnit->IsBitTagged(static_cast<size_t>(ETag::DETECTOR))))
    {
        physicsUnit->EndCollision(shared_from_this());
    }
}

void CUnit::Update(float dt)
{
    CPhysicsUnit::Update(dt);

    UpdateHealthBar();
}

void CUnit::CustomReset()
{
    CPhysicsUnit::CustomReset();

    m_health = m_maxHealth;
    m_healthBar->Show();
    UpdateHealthBar();
}

void CUnit::Paint(Color color)
{
    m_healthBar->SetColor(color);

    SetChildrenColors(color);
}

void CUnit::Heal(HealthType health)
{
    CHealable::Heal(health);
    if (m_health / GetMaxHealth() >= m_smokeCoef)
    {
        m_smoke.Hide();
    }
}

void CUnit::CustomDestroy()
{
    m_smoke.Hide();
    m_healthBar->Hide();

    CPhysicsUnit::CustomDestroy();
}

void CUnit::UpdateHealthBar()
{
    float y = GetPosition().Y + m_healthBar->GetSize().Y * 0.5f + GetSize().Y * 0.5f + 0.2f;
    m_healthBar->SetPosition(Vector2{ GetPosition().X, y });
    m_healthBar->SetValue(m_health / m_maxHealth);
}

void CUnit::UpdateSmokePosition()
{
    if (m_smoke.Added)
    {
        m_smoke.m_actor.SetPosition(GetPosition());
    }
}

void CControlableUnit::HandleAccelerationSignal(float signal)
{
    SetAccelerationMultiplyer(signal);
}

void CControlableUnit::HandleRotationSignal(float signal)
{
    SetRotationMultiplyer(signal);
}

float CControlableUnit::GetHorizontalFriction() const
{
    return m_horizontalFriction;
}

void CControlableUnit::SetHorizontalFriction(float friction)
{
    m_horizontalFriction = friction;
}

float CControlableUnit::GetAcceleration() const
{
    return m_acceleration;
}

void CControlableUnit::SetAcceleration(float acceleration)
{
    m_acceleration = acceleration;
}

float CControlableUnit::GetMaxForwardSpeed() const
{
    return m_maxForwardSpeed;
}

void CControlableUnit::SetMaxForwardSpeed(float maxFwdSpeed)
{
    m_maxForwardSpeed = maxFwdSpeed;
}
float CControlableUnit::GetMaxBackwardSpeed() const
{
    return m_maxBackwardSpeed;
}

void CControlableUnit::SetMaxBackwardSpeed(float maxBwdSpeed)
{
    m_maxBackwardSpeed = maxBwdSpeed;
}

float CControlableUnit::GetRotationSpeed() const
{
    return m_rotationSpeed;
}

void CControlableUnit::SetRotationSpeed(float rotationSpeed)
{
    m_rotationSpeed = rotationSpeed;
}

float CControlableUnit::GetAccelerationMultiplyer() const
{
    return m_accelerationMultiplyer;
}

void CControlableUnit::SetAccelerationMultiplyer(float multiplyer)
{
    m_accelerationMultiplyer = multiplyer;
}

float CControlableUnit::GetRotationMultiplyer() const
{
    return m_rotationMultiplyer;
}

void CControlableUnit::SetRotationMultiplyer(float multiplyer)
{
    m_rotationMultiplyer = multiplyer;
}

void CControlableUnit::Update(float dt)
{
    CUnit::Update(dt);
    UpdateSmokePosition();

    float velocityProjection = GetVelocity().X * cos(MathUtil::ToRadians(GetRotation())) + GetVelocity().Y * sin(MathUtil::ToRadians(GetRotation()));

    if (GetVelocity().Length() > 0)
    {
        ApplyForce(Vector2(-GetHorizontalFriction() * GetVelocity().X, -GetHorizontalFriction() * GetVelocity().Y), Vector2());
    }

    if (velocityProjection < GetMaxForwardSpeed() && velocityProjection > -GetMaxBackwardSpeed())
    {
        ApplyForce(Vector2(GetAcceleration() * GetAccelerationMultiplyer() * cos(GetRotation() * MathUtil::Pi / 180), GetAcceleration() * GetAccelerationMultiplyer() * sin(GetRotation() * MathUtil::Pi / 180)), Vector2());
    }

    velocityProjection = MathUtil::Clamp(velocityProjection, -GetMaxBackwardSpeed(), GetMaxForwardSpeed());
  
    GetBody()->SetAngularVelocity(GetRotationSpeed() * GetRotationMultiplyer());
    GetBody()->SetLinearVelocity(b2Vec2(velocityProjection * cos(MathUtil::ToRadians(GetRotation())), velocityProjection * sin(MathUtil::ToRadians(GetRotation()))));
}

void CControlableUnit::CustomReset()
{
    CUnit::CustomReset();

    m_accelerationMultiplyer = 0;
    m_rotationMultiplyer = 0;
}

void CUnit::SSmoke::Show(Vector2 pos)
{
    if (!Added)
    {
        m_actor = ParticleActor{};
        m_actor.SetPosition(pos);

        luabridge::lua_State* L = theLuaState("Config/smoke.lua");

        auto color = luabridge::getGlobal(L, "color");
        auto endColor = luabridge::getGlobal(L, "endColor");
        auto sprite = luabridge::getGlobal(L, "sprite").cast<std::string>();
        auto length = luabridge::getGlobal(L, "size")["length"].cast<float>();
        auto width = luabridge::getGlobal(L, "size")["width"].cast<float>();
        auto maxParticles = luabridge::getGlobal(L, "maxParticles").cast<size_t>();
        auto emissionRate = luabridge::getGlobal(L, "emissionRate").cast<float>();
        auto lifeTime = luabridge::getGlobal(L, "lifeTime").cast<float>();
        auto spreadAngle = luabridge::getGlobal(L, "spreadAngle").cast<float>();
        auto endScale = luabridge::getGlobal(L, "endScale").cast<float>();
        auto attractorStrength = luabridge::getGlobal(L, "attractorStrength").cast<float>();
        auto minSpeed = luabridge::getGlobal(L, "minSpeed").cast<float>();
        auto maxSpeed = luabridge::getGlobal(L, "maxSpeed").cast<float>();

        m_actor.SetColor(color.At("red").cast<float>(), color.At("green").cast<float>(), color.At("blue").cast<float>(), color.At("alpha").cast<float>());
        m_actor.SetEndColor(Color(endColor.At("red").cast<float>(), endColor.At("green").cast<float>(), endColor.At("blue").cast<float>(), endColor.At("alpha").cast<float>()));
        m_actor.SetSize(length, width);
        m_actor.SetSprite(sprite);
        m_actor.SetMaxParticles(maxParticles);
        m_actor.SetParticlesPerSecond(emissionRate);
        m_actor.SetParticleLifetime(lifeTime);
        m_actor.SetSpread(spreadAngle);
        m_actor.SetEndScale(endScale);
        m_actor.SetAttractorStrength(attractorStrength);
        m_actor.SetSpeedRange(minSpeed, maxSpeed);
        m_actor.SetGravity(Vector2::Zero);

        Added = true;
        theWorld.Add(&m_actor, static_cast<int>(ELayer::GAME_UI) - 1);
    }
}

void CUnit::SSmoke::Hide()
{
    if (Added)
    {
        Added = false;
        theWorld.Remove(&m_actor);
    }
}
