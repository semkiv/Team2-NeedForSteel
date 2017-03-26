#include "stdafx.h"
#include "Team.h"
#include "Tools.h"

CTeam::CTeam(ETeam teamType):
    m_teamType(teamType)
{
}

CTeam::~CTeam()
{
    int l = 0;
}

void CTeam::AddTeamMember(std::shared_ptr<CTeamMember> member)
{    
    member->SetTeam(shared_from_this());
    member->Paint(m_teamColor);
    m_teamMembers.push_back(member);
}

ETeam CTeam::GetTeamType() const
{
    return m_teamType;
}

void CTeam::AddEnemyTeam(std::shared_ptr<CTeam> team)
{
    m_enemiesMask |= static_cast<MaskType>(1) << static_cast<MaskType>(team->GetTeamType());
}

CTeam::MaskType CTeam::GetEnemiesMask() const
{
    return m_enemiesMask;
}

void CTeam::RemoveEnemyTeam(std::shared_ptr<CTeam> team)
{
    m_enemiesMask &= ~(static_cast<MaskType>(1) << static_cast<MaskType>(team->GetTeamType()));
}

bool CTeam::IsEnemy(std::shared_ptr<CTeam> team) const
{
    return (m_enemiesMask & (static_cast<MaskType>(1) << static_cast<MaskType>(team->GetTeamType()))) != 0;
}

void CTeam::SetTeamColor(Color teamColor)
{
    m_teamColor = teamColor;
}

Color CTeam::GetTeamColor() const
{
    return m_teamColor;
}

std::shared_ptr<CTeamShop> CTeam::GetTeamShop() const
{
    return m_teamShop;
}

void CTeam::SetTeamShop(std::shared_ptr<CTeamShop> teamShop)
{
    m_teamShop = teamShop;
}

//void CTeam::SetHeadQuarter(std::shared_ptr<CHeadQuarter> hq)
//{
//    hq->SetTeam(shared_from_this());
//    m_headQuarter = hq;
//}
//
//std::weak_ptr<CHeadQuarter> CTeam::GetHeadQuarter() const
//{
//    return m_headQuarter;
//}

void CTeamManager::AddTeam(std::shared_ptr<CTeam> team)
{
    m_teams[static_cast<size_t>(team->GetTeamType())] = team;
}

std::shared_ptr<CTeam> CTeamManager::GetTeam(ETeam team) const
{
    auto iter = m_teams.find(static_cast<size_t>(team));
    if (iter == m_teams.end())
    {
        return nullptr;
    }

    return iter->second;
}

std::shared_ptr<CTeam> CTeamManager::GetTeam(size_t index) const
{
    auto iter = m_teams.find(index);
    if (iter == m_teams.end())
    {
        return nullptr;
    }

    return iter->second;
}

void CTeamManager::Update(float dt)
{
    for (auto& team : m_teams)
    {
        if (team.second->GetTeamShop() != nullptr)
        {
            team.second->GetTeamShop()->UpdateState();
        }
    }
}

ETeam StringToTeam(String const& str)
{
    ETeam team = ETeam::BLUE;

    if (str == "BLUE")
    {
        team = ETeam::BLUE;
    }
    else if (str == "RED")
    {
        team = ETeam::RED;
    } 
    else if (str == "NEUTRAL")
    {
        team = ETeam::NEUTRAL;
    }

    return team;
}

CTeamShop::CTeamShop(std::shared_ptr<CShop> shop, Vector2 position, float radius):
    m_shop(shop),
    m_position(position),
    m_detectionRadius(radius)
{
}

CTeamShop::~CTeamShop()
{
    int l = 0;
}

std::shared_ptr<CShop> CTeamShop::GetShop() const
{
    return m_shop;
}

bool CTeamShop::IsMagazineEnabled(Vector2 position) const
{
    return (position - m_position).LengthSquared() <= m_detectionRadius * m_detectionRadius;
}

void CTeamShop::SetShopUI(std::shared_ptr<CShopUI> shopUI)
{
    shopUI->SetUpgradePrice(m_shop);
    m_shopUI = shopUI;
}

void CTeamShop::UpdateState()
{
    auto detector = m_detector.lock();
    if (detector == nullptr)
    {
        return;
    }

    auto shopUI = m_shopUI.lock();
    if (shopUI == nullptr)
    {
        return;
    }

    if (IsMagazineEnabled(detector->GetPosition()))
    {
        if (!shopUI->IsActive())
        {
            shopUI->Show();
        }
    }
    else
    {
        if (shopUI->IsActive())
        {
            shopUI->Hide();
        }
    }
}

void CTeamShop::SetDetector(std::shared_ptr<Actor> detector)
{
    m_detector = detector;
}

size_t CTeamShop::GetSelectedUpgrade() const
{
    return m_shopUI.lock()->CurrentIcon();
}

bool CTeamShop::IsActive() const
{
    auto shopUI = m_shopUI.lock();
    return shopUI != nullptr && shopUI->IsActive();
}

void CTeamShop::MoveUp()
{
    m_shopUI.lock()->AcceptCommand(ENavigationCommand::UP);
}

void CTeamShop::MoveDown()
{
    m_shopUI.lock()->AcceptCommand(ENavigationCommand::DOWN);
}

void CTeamShop::Upgrade()
{
    m_shopUI.lock()->AcceptCommand(ENavigationCommand::UPGRADE);
}
