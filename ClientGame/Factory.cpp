#include "stdafx.h"
#include "Factory.h"
#include "Turret.h"
#include "TargetDetection.h"
#include "Tank.h"
#include "Tower.h"
#include "HeadQuarter.h"
#include "Mine.h"
#include "LuaManager.h"

CFactory* CFactory::s_instance = nullptr;

CFactory& CFactory::GetInstance()
{
    if (s_instance == nullptr)
    {
        s_instance = new CFactory{};
    }
    return *s_instance;
}

template<>
std::shared_ptr<CTurret> CFactory::CreateFromLua(std::string const& configFile)
{
    luabridge::lua_State* L = theLuaState(configFile);

    auto sprite = luabridge::getGlobal(L, "sprite").cast<std::string>();
    auto length = luabridge::getGlobal(L, "size")["length"].cast<float>();
    auto width = luabridge::getGlobal(L, "size")["width"].cast<float>();
    auto rotSpeed = luabridge::getGlobal(L, "rotationSpeed").cast<float>();
    auto coolDown = luabridge::getGlobal(L, "coolDown").cast<float>();
    auto projectileFD = luabridge::getGlobal(L, "projectileFlightDistance").cast<float>();
    auto projectileSpeed = luabridge::getGlobal(L, "projectileSpeed").cast<float>();
    auto dmg = luabridge::getGlobal(L, "damage").cast<float>();
    auto dmgType = static_cast<EDamageType>(luabridge::getGlobal(L, "damageType").cast<int>());
    auto projectile = luabridge::getGlobal(L, "projectile").cast<std::string>();

    auto turret = theFactory.Create<CTurret>(ELayer::TURRET);

    turret->SetSprite(sprite);
    turret->SetSize(length, width);
    turret->SetRotationSpeed(rotSpeed);
    turret->SetRotationMultiplyer(0.0f);
    turret->SetCoolDown(coolDown);
    turret->SetProjectileFlightDistance(projectileFD);
    turret->SetProjectileSpeed(projectileSpeed);
    turret->SetDamage(dmg);
    turret->SetDamageType(dmgType);
    turret->SetRelativeRotation(0.0f);
    turret->SetProjectile(projectile);

    return turret;
}

template<>
std::shared_ptr<CTargetTurret> CFactory::CreateFromLua(std::string const& configFile)
{
    luabridge::lua_State* L = theLuaState(configFile);

    auto sprite = luabridge::getGlobal(L, "sprite").cast<std::string>();
    auto length = luabridge::getGlobal(L, "size")["length"].cast<float>();
    auto width = luabridge::getGlobal(L, "size")["width"].cast<float>();
    auto rotSpeed = luabridge::getGlobal(L, "rotationSpeed").cast<float>();
    auto coolDown = luabridge::getGlobal(L, "coolDown").cast<float>();
    auto projectileFD = luabridge::getGlobal(L, "projectileFlightDistance").cast<float>();
    auto projectileSpeed = luabridge::getGlobal(L, "projectileSpeed").cast<float>();
    auto dmg = luabridge::getGlobal(L, "damage").cast<float>();
    auto dmgType = static_cast<EDamageType>(luabridge::getGlobal(L, "damageType").cast<int>());
    auto projectile = luabridge::getGlobal(L, "projectile").cast<std::string>();

    auto turret = theFactory.Create<CTargetTurret>(ELayer::TURRET);

    turret->SetSprite(sprite);
    turret->SetSize(length, width);
    turret->SetRotationSpeed(rotSpeed);
    turret->SetRotationMultiplyer(0.0f);
    turret->SetCoolDown(coolDown);
    turret->SetProjectileFlightDistance(projectileFD);
    turret->SetProjectileSpeed(projectileSpeed);
    turret->SetDamage(dmg);
    turret->SetDamageType(dmgType);
    turret->SetProjectile(projectile);
    turret->SetRelativeRotation(0.0f);

    return turret;
}

template<>
std::shared_ptr<CTargetDetector> CFactory::CreateFromLua(std::string const& configFile)
{
    luabridge::lua_State* L = theLuaState(configFile);

    auto length = luabridge::getGlobal(L, "size")["length"].cast<float>();
    auto width = luabridge::getGlobal(L, "size")["width"].cast<float>();

    auto detector = theFactory.Create<CTargetDetector>();

    detector->SetSize(length, width);
    detector->SetAlpha(0.0f);

    detector->SetDrawShape(actorDrawShape::ADS_Circle);
    detector->SetShapeType(PhysicsActor::eShapeType::SHAPETYPE_CIRCLE);
    detector->SetIsSensor(true);

    return detector;
}

template<>
std::shared_ptr<CTank> CFactory::CreateFromLua(std::string const& configFile)
{
    luabridge::lua_State* L = theLuaState(configFile);

    auto sprite = luabridge::getGlobal(L, "sprite").cast<std::string>();
    auto maxHP = luabridge::getGlobal(L, "maxHealth").cast<CHealable::HealthType>();
    auto maxFwdSpeed = luabridge::getGlobal(L, "maxForwardSpeed").cast<float>();
    auto maxBwdSpeed = luabridge::getGlobal(L, "maxBackwardSpeed").cast<float>();
    auto accel = luabridge::getGlobal(L, "acceleration").cast<float>();
    auto rotSpeed = luabridge::getGlobal(L, "rotationSpeed").cast<float>();
    auto hFriction = luabridge::getGlobal(L, "horizontalFriction").cast<float>();
    auto length = luabridge::getGlobal(L, "size")["length"].cast<float>();
    auto width = luabridge::getGlobal(L, "size")["width"].cast<float>();
    auto density = luabridge::getGlobal(L, "density").cast<float>();
    auto friction = luabridge::getGlobal(L, "friction").cast<float>();
    auto restitution = luabridge::getGlobal(L, "restitution").cast<float>();
    auto feedBack = luabridge::getGlobal(L, "feedBack").cast<float>();
    auto turretConfigFile = luabridge::getGlobal(L, "turret").cast<std::string>();
    auto healMultiplayer = luabridge::getGlobal(L, "healMultiplayer").cast<float>();
    auto dmgType = static_cast<EDamageType>(luabridge::getGlobal(L, "damageType").cast<int>());
    auto worth = luabridge::getGlobal(L, "worth").cast<size_t>();

    auto tank = theFactory.Create<CTank>(ELayer::UNIT);

    tank->SetSprite(sprite);
    tank->SetMaxHealth(maxHP);
    tank->Heal(maxHP);
    tank->SetMaxForwardSpeed(maxFwdSpeed);
    tank->SetMaxBackwardSpeed(maxBwdSpeed);
    tank->SetAcceleration(accel);
    tank->SetRotationSpeed(rotSpeed);
    tank->SetHorizontalFriction(hFriction);
    tank->SetSize(length, width);
    tank->SetDensity(density);
    tank->SetFriction(friction);
    tank->SetRestitution(restitution);
    tank->SetFeedBack(feedBack);
    tank->SetDamageType(dmgType);
    tank->SetWorth(worth);
    tank->SetShapeType(PhysicsActor::eShapeType::SHAPETYPE_BOX);    
    tank->SetDrawShape(actorDrawShape::ADS_Square);
    tank->SetHealMultiplyer(healMultiplayer);

    auto turret = CreateFromLua<CTurret>(turretConfigFile);
    tank->SetTurret(turret);

    return tank;
}

template<>
std::shared_ptr<CTower> CFactory::CreateFromLua(std::string const& configFile)
{
    luabridge::lua_State* L = theLuaState(configFile);

    auto sprite = luabridge::getGlobal(L, "sprite").cast<std::string>();
    auto maxHP = luabridge::getGlobal(L, "maxHealth").cast<CHealable::HealthType>();
    auto length = luabridge::getGlobal(L, "size")["length"].cast<float>();
    auto width = luabridge::getGlobal(L, "size")["width"].cast<float>();
    auto restitution = luabridge::getGlobal(L, "restitution").cast<float>();
    auto turretConfigFile = luabridge::getGlobal(L, "turret").cast<std::string>();
    auto detectorConfigFile = luabridge::getGlobal(L, "detector").cast<std::string>();
    auto dmgType = static_cast<EDamageType>(luabridge::getGlobal(L, "damageType").cast<int>());
    auto healMultiplayer = luabridge::getGlobal(L, "healMultiplayer").cast<float>();
    auto worth = luabridge::getGlobal(L, "worth").cast<size_t>();

    auto tower = theFactory.Create<CTower>(ELayer::TOWER);

    tower->SetShapeType(PhysicsActor::eShapeType::SHAPETYPE_CIRCLE);
    tower->SetDensity(0.0f);
    tower->SetMaxHealth(maxHP);
    tower->Heal(maxHP);
    tower->SetSize(length, width);
    tower->SetSprite(sprite);
    tower->SetRestitution(restitution);
    tower->SetDamageType(dmgType);
    tower->SetWorth(worth);
    tower->SetHealMultiplyer(healMultiplayer);

    auto turret = CreateFromLua<CTargetTurret>(turretConfigFile);
    tower->SetTurret(turret);
    auto targetDetector = CreateFromLua<CTargetDetector>(detectorConfigFile);
    tower->SetDetector(targetDetector);

    return tower;
}

template<>
std::shared_ptr<CHeadQuarter> CFactory::CreateFromLua(std::string const& configFile)
{
    luabridge::lua_State* L = theLuaState(configFile);

    auto sprite = luabridge::getGlobal(L, "sprite").cast<std::string>();
    auto maxHP = luabridge::getGlobal(L, "maxHealth").cast<CHealable::HealthType>();
    auto length = luabridge::getGlobal(L, "size")["length"].cast<float>();
    auto width = luabridge::getGlobal(L, "size")["width"].cast<float>();
    auto restitution = luabridge::getGlobal(L, "restitution").cast<float>();
    auto detectorConfigFile = luabridge::getGlobal(L, "detector").cast<std::string>();
    auto dmgType = static_cast<EDamageType>(luabridge::getGlobal(L, "damageType").cast<int>());
    auto worth = luabridge::getGlobal(L, "worth").cast<size_t>();
    auto healMultiplayer = luabridge::getGlobal(L, "healMultiplayer").cast<float>();
    auto healingPower = luabridge::getGlobal(L, "healingPower").cast<float>();

    auto hq = theFactory.Create<CHeadQuarter>(ELayer::BASE);

    hq->SetDensity(0.0f);
    hq->SetMaxHealth(maxHP);
    hq->Heal(maxHP);
    hq->SetSize(length, width);
    hq->SetSprite(sprite);
    hq->SetRestitution(restitution);
    hq->SetDamageType(dmgType);
    hq->SetWorth(worth);
    hq->SetShapeType(PhysicsActor::eShapeType::SHAPETYPE_CIRCLE);
    hq->SetHealingPower(healingPower);
    hq->SetHealMultiplyer(healMultiplayer);

    auto targetDetector = CreateFromLua<CTargetDetector>(detectorConfigFile);
    hq->SetDetector(targetDetector);

    return hq;
}

template<>
std::shared_ptr<CMine> CFactory::CreateFromLua(std::string const& configFile)
{
    luabridge::lua_State* L = theLuaState(configFile);

    auto sprite = luabridge::getGlobal(L, "sprite").cast<std::string>();
    auto maxHP = luabridge::getGlobal(L, "maxHealth").cast<CHealable::HealthType>();
    auto length = luabridge::getGlobal(L, "size")["length"].cast<float>();
    auto width = luabridge::getGlobal(L, "size")["width"].cast<float>();
    auto maxFwdSpeed = luabridge::getGlobal(L, "maxForwardSpeed").cast<float>();
    auto maxBwdSpeed = luabridge::getGlobal(L, "maxBackwardSpeed").cast<float>();
    auto accel = luabridge::getGlobal(L, "acceleration").cast<float>();
    auto rotSpeed = luabridge::getGlobal(L, "rotationSpeed").cast<float>();
    auto hFriction = luabridge::getGlobal(L, "horizontalFriction").cast<float>();    
    auto density = luabridge::getGlobal(L, "density").cast<float>();
    auto friction = luabridge::getGlobal(L, "friction").cast<float>();
    auto restitution = luabridge::getGlobal(L, "restitution").cast<float>();
    auto dmgRadius = luabridge::getGlobal(L, "damageRadius").cast<float>();
    auto dmg = luabridge::getGlobal(L, "damage").cast<float>();
    auto explosionDmg = luabridge::getGlobal(L, "explosionDamage").cast<float>();
    auto dmgType = static_cast<EDamageType>(luabridge::getGlobal(L, "damageType").cast<int>());
    auto worth = luabridge::getGlobal(L, "worth").cast<size_t>();

    auto mine = theFactory.Create<CMine>(ELayer::UNIT);

    mine->SetSprite(sprite);
    mine->SetMaxHealth(maxHP);
    mine->Heal(maxHP);
    mine->SetMaxForwardSpeed(maxFwdSpeed);
    mine->SetMaxBackwardSpeed(maxBwdSpeed);
    mine->SetAcceleration(accel);
    mine->SetRotationSpeed(rotSpeed);
    mine->SetHorizontalFriction(hFriction);
    mine->SetSize(length, width);   
    mine->SetDensity(density);
    mine->SetFriction(friction);
    mine->SetRestitution(restitution);
    mine->SetDamageRadius(dmgRadius);
    mine->SetExplosionDamage(explosionDmg);
    mine->SetDamage(dmg);
    mine->SetDamageType(dmgType);
    mine->SetWorth(worth);

    mine->SetShapeType(PhysicsActor::eShapeType::SHAPETYPE_CIRCLE);
    mine->SetDrawShape(actorDrawShape::ADS_Circle);

    return mine;
}

template<>
std::shared_ptr<CProjectile> CFactory::CreateFromLua(std::string const& configFile)
{
    luabridge::lua_State* L = theLuaState(configFile);

    auto sprite = luabridge::getGlobal(L, "sprite").cast<std::string>();
    auto length = luabridge::getGlobal(L, "size")["length"].cast<float>();
    auto width = luabridge::getGlobal(L, "size")["width"].cast<float>();
    auto worth = luabridge::getGlobal(L, "worth").cast<size_t>();

    auto projectile = theFactory.Create<CProjectile>(ELayer::PROJECTILE);

    projectile->SetSprite(sprite);
    projectile->SetSize(length, width);
    projectile->SetWorth(worth);
    
    return projectile;
}

template<>
std::shared_ptr<CTargetProjectile> CFactory::CreateFromLua(std::string const& configFile)
{
    luabridge::lua_State* L = theLuaState(configFile);

    auto sprite = luabridge::getGlobal(L, "sprite").cast<std::string>();
    auto length = luabridge::getGlobal(L, "size")["length"].cast<float>();
    auto width = luabridge::getGlobal(L, "size")["width"].cast<float>();
    auto worth = luabridge::getGlobal(L, "worth").cast<size_t>();

    auto projectile = theFactory.Create<CTargetProjectile>(ELayer::PROJECTILE);

    projectile->SetSprite(sprite);
    projectile->SetSize(length, width);
    projectile->SetWorth(worth);

    return projectile;
}
