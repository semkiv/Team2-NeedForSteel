#pragma once

#include "Angel.h"
#include "Tank.h"
#include "Shop.h"
#include "Team.h"
#include <vector>
#include <memory>

class CPlayer;

class IPlayerListener
{
public:
    virtual void OnGoldChanged(size_t gold) {}
    virtual void OnShoot() {}
    virtual void OnTakeDamage() {}
};

class CPlayer : public IPlayerInputListener,
    public IDamagerListener,
    public IDamageableListener,
    public std::enable_shared_from_this<CPlayer>
{

public:
	CPlayer(std::shared_ptr<CTeamShop> shop);
    virtual ~CPlayer();

	void SetTank(std::shared_ptr<CTank> tank);
    std::weak_ptr<CTank> GetTank() const;

	void SetGold(size_t gold);
    size_t GetGold() const;

	virtual void OnPlayerKeyPress(SPlayerDigitalInputEvent digitInput, SPlayerInputBinding const& binding) override;
	virtual void OnPlayerKeyRelease(SPlayerDigitalInputEvent digitInput, SPlayerInputBinding const& binding) override;
	virtual void OnPlayerAnalogInput(SPlayerAnalogInputEvent analogInput, SPlayerInputBinding const& binding) override;

    virtual void OnKilled(std::weak_ptr<CDamageable> victim) override;
    virtual void OnTakeDamage() override;

    void AddPlayerListener(std::weak_ptr<IPlayerListener> listener);

private:
    using MaskType = size_t;

    std::vector<int> m_currentUpgradeLevel;
    std::weak_ptr<CTeamShop> m_shop;
	std::weak_ptr<CTank> m_tank;

    size_t m_gold;
	float m_controlSensitivity;
    std::vector<std::weak_ptr<IPlayerListener>> m_listeners;

    MaskType m_inputMask{};

private:
    void TankShoot(float signal);
    bool IsPressed(EPlayerInputEventType input);
    void UpdateInputMask(EPlayerInputEventType input, bool pressed);
    void TryUpgradeTank(EUpgrade upgrade);
    void UpgradeTank(EUpgrade upgrade);

    
};

class CPlayerManager
{
public:
    virtual ~CPlayerManager();

    size_t PlayersCount() const;
    void AddPlayer(std::shared_ptr<CPlayer> player);
    std::shared_ptr<CPlayer> GetPlayer(size_t playerId) const;

private:
    std::vector<std::shared_ptr<CPlayer>> m_players;
};
