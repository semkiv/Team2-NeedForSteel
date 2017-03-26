#pragma once

#include "Angel.h"
#include "Common.h"
#include "Interfaces.h"
#include "HeadQuarter.h"
#include "Shop.h"
#include "ShopUI.h"
#include <vector>
#include <memory>
#include <unordered_map>

class CPlayer;

enum class ETeam
{
    RED = 0,
    BLUE = 1,
    NEUTRAL = 2,
    NONE = 10000
};

class CTeamShop
{
public:
    CTeamShop(std::shared_ptr<CShop> shop,  Vector2 position, float radius);
    virtual ~CTeamShop();

    std::shared_ptr<CShop> GetShop() const;
    bool IsMagazineEnabled(Vector2 position) const;

    void SetShopUI(std::shared_ptr<CShopUI> shopUI);
    void UpdateState();
    void SetDetector(std::shared_ptr<Actor> detector);

    size_t GetSelectedUpgrade() const;
    bool IsActive() const;
    void MoveUp();
    void MoveDown();
    void Upgrade();

private:
    std::shared_ptr<CShop> m_shop;
    std::weak_ptr<CShopUI> m_shopUI;
    std::weak_ptr<Actor> m_detector;
    Vector2 m_position;
    float m_detectionRadius;
};

class CTeam : public std::enable_shared_from_this<CTeam>
{
public:
    using MaskType = size_t;
    
    CTeam(ETeam teamType);
    virtual ~CTeam();
    
    void AddTeamMember(std::shared_ptr<CTeamMember> member);

    ETeam   GetTeamType() const;
    void    AddEnemyTeam(std::shared_ptr<CTeam> team);
    MaskType  GetEnemiesMask() const;
    void    RemoveEnemyTeam(std::shared_ptr<CTeam> team);
    bool    IsEnemy(std::shared_ptr<CTeam> team) const;

    void SetTeamColor(Color color);
    Color GetTeamColor() const;

    std::shared_ptr<CTeamShop> GetTeamShop() const;
    void SetTeamShop(std::shared_ptr<CTeamShop> teamShop);

private:
    MaskType m_enemiesMask{};
    ETeam m_teamType;
    Color m_teamColor;
    std::vector<std::weak_ptr<CTeamMember>> m_teamMembers;
    std::shared_ptr<CTeamShop> m_teamShop;
};

class CTeamManager
{
public:
    void AddTeam(std::shared_ptr<CTeam> team);
    std::shared_ptr<CTeam> GetTeam(ETeam team) const;
    std::shared_ptr<CTeam> GetTeam(size_t index) const;

    void Update(float dt);

private:
    std::unordered_map<size_t, std::shared_ptr<CTeam>> m_teams;
};

ETeam StringToTeam(String const& str);
